#pragma once

#include <tuple>
#include <utils/files.hpp>
#include <utils/vector.hpp>

#include "../Hardware/commands.hpp"

namespace qasm{
    using namespace utils;

    using blocksBigType = std::map<
        std::string, // block name 
        std::vector< // all block commands
            std::tuple<
                int,                     // command index
                std::string,             // command name
                std::vector<std::string> // command arguments (optional)
            >
        >
    >;
}