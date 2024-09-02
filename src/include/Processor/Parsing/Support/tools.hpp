#pragma once

#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <termios.h>

#include "../definitions.hpp"

namespace qasm{

    std::vector<std::string> getConnected(const std::vector<std::string>& lines, const std::string& basePath = "") {
        std::vector<std::string> output;
        
        for (const auto& line : lines) {
            if (line.empty() || line[0] != '$') {
                output.push_back(line);
                continue;
            }

            // Обработка подключения файла
            std::string filePath = line.substr(1);  // Убираем символ $
            if (!basePath.empty()) {
                filePath = std::filesystem::path(basePath) / filePath;
            }

            std::ifstream file(filePath);
            if (!file.is_open()) {
                std::cerr << "Ошибка: Не удалось открыть файл " << filePath << std::endl;
                continue;
            }

            std::vector<std::string> fileLines;
            std::string fileLine;
            while (std::getline(file, fileLine)) {
                fileLines.push_back(fileLine);
            }

            // Рекурсивно обрабатываем подключенный файл
            std::string newBasePath = std::filesystem::path(filePath).parent_path().string();
            auto connectedLines = getConnected(fileLines, newBasePath);
            output.insert(output.end(), connectedLines.begin(), connectedLines.end());
        }

        return output;
    }

    std::map<std::string, std::string> getConfVariables(std::vector<std::string> lines){
        std::map<std::string, std::string> output;

        for(auto &line: lines){
            if(line[0] == '['){
                std::string name, value;

                auto spltd = utils::vec::stripsplit(utils::str::slice(line, 1, -1), ':');
                name = spltd[0];
                value = spltd[1];
                
                output[name] = value;
            }
        }

        return output;
    }

    void printBlockBigType(
        std::map<
            std::string, // block name 
            std::vector< // all block commands
                std::tuple<
                    int,                     // command index
                    std::string,             // command name
                    std::vector<std::string> // command arguments (optional)
                >
            >
        > blocks
    ){
        for(auto &block: blocks){
            std::cout << block.first << std::endl;
            for(auto &command: block.second){
                std::cout << "  " << std::get<0>(command) << " | ";
                std::cout << std::get<1>(command) << ' ';
                if(!std::get<2>(command).empty()){
                    std::cout << "[";
                    for(auto &arg: std::get<2>(command))
                        std::cout << arg << " ";
                    std::cout << ']' << std::endl;
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }

    char getch(void){
        char buf = 0;
        struct termios old = {0};
        fflush(stdout);
        if(tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if(tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if(read(0, &buf, 1) < 0)
            perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if(tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");
        return buf;
    }
}