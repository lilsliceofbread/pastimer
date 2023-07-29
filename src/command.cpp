#include <map>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include "util.hpp"
#include "command.hpp"

typedef void (*cmd_handler)(const std::string &);

static std::map<std::string, cmd_handler> cmap;

const static std::string extension = ".task";
static std::string config_filepath = "";

void add_handler(const std::string &argument)
{
    std::string filepath;

    filepath = config_filepath + argument + extension;

    if (std::ifstream(filepath))
    {
        std::cout << "task already exists" << std::endl;
        return;
    }

    std::cout << "adding " << argument << " to task config" << std::endl;

    reset_task(filepath);
}

void start_handler(const std::string &argument)
{
    auto curr_time = std::chrono::system_clock::now();
    std::ofstream task_file;
    std::string filepath = config_filepath + argument + extension;

    // open file (read only) w/ position indicator at end
    std::ifstream file_check(filepath, std::ios::ate);
    std::streampos file_check_size = file_check.tellg();

    if (!file_check.is_open())
    {
        std::cout << "task does not exist" << std::endl;
        file_check.close();
        return;
    }

    if(file_check_size <= 2) // should have atleast ~\n~ - 3 chars
    {
        std::cout << "task has no data, resetting config" << std::endl;
        reset_task(filepath);
    } else {
        char last_char = '\n';
        int cursor_pos = -1;
        while (last_char == '\n' && (cursor_pos * -1) < file_check_size)
        {
            file_check.seekg(cursor_pos, std::ios::end); // set position indicator to cursor_pos behind EOF
            file_check.get(last_char);                   // get char at position indicator
            cursor_pos--;
        }
        std::cout << "last character: " << last_char << std::endl;

        if (last_char != '~') // if final char does not equal the end of file
        {
            std::cout << "timer already started" << std::endl;
            // could ask whether to restart timer
            file_check.close();
            return;
        }
    }
    file_check.close();

    task_file.open(filepath, std::ios::app); 
    if(!task_file.is_open())
    {
        std::cout << "could not open task file" << std::endl;
        task_file.close();
        return;
    }

    std::time_t unix_time = std::chrono::system_clock::to_time_t(curr_time);
    std::string time_str = std::ctime(&unix_time);

    task_file << "\n" << time_str; // output starting time to file

    std::cout << "starting timer for " << argument << std::endl;
    std::cout << "current time: " << time_str << std::endl;

    task_file.close();
}

void stop_handler(const std::string &argument)
{
    std::cout << "stopping current timer" << std::endl;
}

void gettime_handler(const std::string &argument)
{
    std::cout << "getting total time for " << argument << std::endl;
}

void init_cmd()
{
    // key of command string, value of pointer to command function
    cmap.emplace("add", &add_handler);
    cmap.emplace("start", &start_handler);
    cmap.emplace("stop", &stop_handler);
    cmap.emplace("time", &gettime_handler);

    config_filepath = get_config_dir();
}

void run_cmd(const std::string &command_str, const std::string &argument)
{
    // iterator containing location of match
    std::map<std::string, cmd_handler>::const_iterator command_iter = cmap.find(command_str);

    if (command_iter == cmap.end())
    {
        std::cout << "no command matches that argument" << std::endl;
        return;
    }

    // get command handler function and run
    cmd_handler current_cmd_handler = command_iter->second;
    current_cmd_handler(argument);
}