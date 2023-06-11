#include <unordered_map>
#include <string>
#include <iostream>
#include "command.hpp"


static std::unordered_map<std::string, CommandType> cmap;

void initCmd() {
    cmap.insert({"add", CommandType::ADD});
    cmap.insert({"start", CommandType::START});
    cmap.insert({"stop", CommandType::STOP});
    cmap.insert({"time", CommandType::GET_TIME});
}

CommandType compareCmd(std::string command_str) {
    std::unordered_map<std::string, CommandType>::const_iterator command_iter = cmap.find(command_str);

    if(command_iter == cmap.end()) {
        std::cout << "no command matches that argument" << std::endl;
        exit(0);
    }
    return command_iter->second;
}
