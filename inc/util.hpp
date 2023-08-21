#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

std::string get_config_dir()
{
    // get linux environment var HOME
    const char* home_dir = getenv("HOME");
    if(home_dir == NULL)
    {
        // get linux user database entry
        home_dir = getpwuid(getuid())->pw_dir;
    }

    std::string config_path = std::string(home_dir) + "/.config/";

    std::filesystem::current_path(config_path);
    if(!std::filesystem::is_directory("pastimer") || !std::filesystem::exists("pastimer")) 
    {
        std::cout << "pastimer config does not exist, creating" << std::endl;
        std::filesystem::create_directory("pastimer");
    }

    std::string path = config_path + "pastimer/";
    return path;
}
#elif _WIN32
std::string get_config_dir() 
{
    // no windows implementation
}
#endif

void reset_task(std::string filepath) {
    std::ofstream task_file;

    task_file.open(filepath, std::ios::trunc);
    if (!task_file.is_open())
    {
        std::cout << "could not create config file" << std::endl;
        task_file.close();
        return;
    }

    task_file << "~\n~";

    task_file.close();
}