#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include "command.hpp"

typedef void(*cmd_handler)(const std::string&);

static std::unordered_map<std::string, cmd_handler> cmap;

const static std::string extension = ".task";
static std::string task_filepath;

void add_handler(const std::string& argument) {
    std::ofstream task_file; 
    std::string filepath;

    std::cout << "adding " << argument << " to task list" << std::endl;

    filepath = task_filepath + argument + extension;
    if(std::ifstream(filepath)) {
        std::cout << "task already exists" << std::endl;
    }

    task_file.open(filepath);
    if(!task_file.is_open()) {
        std::cout << "could not create config file" << std::endl;
    }

    task_file.close();
}

void start_handler(const std::string& argument) {
    std::cout << "starting timer for " << argument << std::endl;
}

void stop_handler(const std::string& argument) {
    std::cout << "stopping current timer" << std::endl;
}

void gettime_handler(const std::string& argument) {
    std::cout << "getting total time for " << argument << std::endl;
}

void config_filepath(const std::string& argument) {
    std::cout << "setting config file path to " << argument << std::endl;
    task_filepath = argument;
}

void init_cmd() {
    //key of command string, value of pointer to command function
    cmap.emplace("add", &add_handler);
    cmap.emplace("start", &start_handler);
    cmap.emplace("stop", &stop_handler);
    cmap.emplace("time", &gettime_handler);

    task_filepath = "";
    //get filepath from config file
}

void run_cmd(const std::string& command_str, const std::string& argument) {
    //iterator containing location of match
    std::unordered_map<std::string, cmd_handler>::const_iterator command_iter = cmap.find(command_str);

    if(command_iter == cmap.end()) {
        std::cout << "no command matches that argument" << std::endl;
        return;
    }

    //get command handler function and run
    cmd_handler current_cmd_handler = command_iter->second;
    current_cmd_handler(argument); 
}