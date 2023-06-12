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

CommandType compareCmd(const std::string& command_str) { 
    //iterator containing location of match
    std::unordered_map<std::string, CommandType>::const_iterator command_iter = cmap.find(command_str);

    if(command_iter == cmap.end()) {
        std::cout << "no command matches that argument" << std::endl;
        exit(0);
    }
    //return CommandType
    return command_iter->second;
}

void runCmd(CommandType command, const std::string& argument) {
    switch(command) {
        case ADD:
            std::cout << "add command run" << std::endl;
            break;
        case START:
            std::cout << "start command run" << std::endl;
            break;
        case STOP:
            std::cout << "stop command run" << std::endl;
            break;
        case GET_TIME:  
            std::cout << "get time command run" << std::endl;
            break;
    }
}