#pragma once

#include "definitions.hpp"

#include "Support/tools.hpp"
#include "Support/cmd-parser.hpp"

namespace qasm{
    class Processor{
            std::string code_filename;
            std::vector<std::string> code_vec;
            std::map<std::string, std::string> conf_vars;
            
            registers __selfregisters;
            newheap   __selfheap;
            std::stack<__STACK_MAIN_TYPE> __selfstack;

            std::map<std::string, int> blocksinfo;
            
        public:

        std::tuple<blocksBigType, int> blockinize(
            int basicIdentation
        ){
            blocksBigType output;

            int indetation_level = 0;
            auto count_indetation = [&](std::string line) -> int { 
                auto indx = line.find_first_not_of(' '); 
                return indx == std::string::npos ? 0: indx;
            };

            std::string current_block = "";
            bool canAddNewToCurrent = true;
            int UCI = 0;
            for(auto &line: code_vec){
                if(line.empty() || vec::stripsplit(line).size() == 0 || vec::stripsplit(line).empty() || str::strip(line)[0] == ';')
                    continue;
                
                int current_indetation = count_indetation(line);
                bool isblockname = line.back() == ':';

                if(isblockname){
                    current_block = line.substr(1, line.size()-2);
                    output[current_block] = {};
                    indetation_level = basicIdentation;
                    blocksinfo[current_block] = UCI;
                    continue;
                }

                if(current_indetation == indetation_level && canAddNewToCurrent){
                    std::string cmdname = vec::stripsplit(line)[0];
                    output[current_block].push_back({});

                    if(str::strip(line) == cmdname){
                        std::get<0>(output[current_block].back()) = UCI;
                        std::get<1>(output[current_block].back()) = cmdname;
                        std::get<2>(output[current_block].back()) = {};
                        UCI++;
                        continue;
                    }

                    std::vector<std::string> arguments = vec::stripsplit(
                        str::strip(line).substr(1 + cmdname.size()), ','
                    );

                    std::get<0>(output[current_block].back()) = UCI;
                    std::get<1>(output[current_block].back()) = cmdname;
                    std::get<2>(output[current_block].back()) = arguments;
                    UCI++;
                }
            }

            return {output, UCI};
        }

        int process(blocksBigType &blocks){
            bool changeIT = false;
            std::string changeTo;
            std::string currentBlockName;
            int counter = 0;
            int __local_counter = 0;

            __selfregisters.rbp = counter;
            auto currentBlock = blocks[conf_vars["global"]];
            currentBlockName  = conf_vars["global"];

            std::vector<std::tuple<std::string, int, int>> parentblocks;

            bool stopping = false;
            while(__local_counter <= std::get<0>(currentBlock.back())){
                stopping = stopping || (currentBlockName == "char");
                changeIT = false;
                changeTo = "...";

                if(currentBlockName == conf_vars["global"] && __local_counter >= currentBlock.size()){
                    break;
                }

                // std::cout << currentBlockName << ' ' << counter << ' ' << __local_counter << ' ' << std::get<0>(currentBlock.back()) << std::endl;
                auto &cmdtuple = currentBlock[__local_counter];
                // std::cout << 1 << std::endl;

                std::cout << "block " << currentBlockName << " localcounter " << __local_counter << " > " << 
                        std::get<0>(cmdtuple) << " " << 
                        std::get<1>(cmdtuple) << " " << 
                        utils::str::to_string(std::get<2>(cmdtuple)) 
                    << std::endl;
                
                if(stopping) 
                    getch();

                auto id =          std::get<0>(cmdtuple);
                auto commandname = std::get<1>(cmdtuple);
                auto arguments =   std::get<2>(cmdtuple);

                int precounter = counter;
                int prelocal =  __local_counter;
                __executebyname(
                    __selfstack,
                    __selfregisters,
                    __selfheap,
                    blocksinfo,
                    commandname,
                    arguments,
                    counter,
                    __local_counter,
                    changeIT,
                    changeTo
                );

                
                // std::cout << currentBlockName << ' ' << std::get<0>(parentblocks.back()) << std::endl;

                if(changeIT && (parentblocks.size() == 0 || (std::get<0>(parentblocks.back()) != currentBlockName))){
                    std::cout << -1 << std::endl;
                    std::cout << "pushing: " << currentBlockName << ' ' << precounter + 3 << ' ' << prelocal +1 << std::endl;
                    parentblocks.push_back({currentBlockName, precounter + 3, prelocal +1});
                    currentBlock = blocks[changeTo];
                    currentBlockName = changeTo;
                } else if(changeIT && (changeTo == currentBlockName)){
                    std::cout << -2 << std::endl;
                    __local_counter = 0;
                } else {
                    __local_counter++;
                    counter++;
                }
                std::cout << "changeIT: " << ((changeIT) ? "True": "False") << " to " << changeTo << std::endl;
                utils::io::Print::print(parentblocks);

                if(currentBlockName != conf_vars["global"] && __local_counter >= currentBlock.size()){
                    std::cout << "OUT OF BLOCK " << currentBlockName << std::endl;
                    utils::io::Print::print(parentblocks);
                    auto info = parentblocks.back();
                    parentblocks.pop_back();
                    
                    currentBlockName = std::get<0>(info);
                    currentBlock = blocks[currentBlockName];

                    std::cout << "NEW BLOCK: " << currentBlockName << std::endl;

                    counter = std::get<1>(info);
                    __local_counter = std::get<2>(info);
                    std::cout << "--<< " << counter << ' ' << __local_counter << ' ' << currentBlockName << std::endl;
                    std::cout << "--<< " << std::get<0>(currentBlock.back()) << ' ' << currentBlockName << std::endl;
                    std::cout << "--<< " << std::get<1>(currentBlock[__local_counter]) << std::endl;
                }
                std::cout << "********" << std::endl;
            }

            return 0;
        }

        Processor(
            std::string code_filename
        ): code_filename(code_filename), code_vec(getConnected(
            utils::vec::split(
                utils::fls::getFile(code_filename),
                '\n'),
            std::filesystem::path(code_filename).parent_path()
        )){
            conf_vars = getConfVariables(code_vec);
            // utils::io::Print::print(code_vec, '\n');
        }

        Processor(){}
        ~Processor(){}        
    };
}
