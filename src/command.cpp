#include <map>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include "util.hpp"
#include "command.hpp"

typedef void(*cmd_handler)(const std::string (&arguments)[2]);

static std::map<std::string, cmd_handler> cmap;

const static std::string extension = ".task";
static std::string config_filepath = "";

void add_handler(const std::string (&arguments)[2])
{
    std::string filepath;

    filepath = config_filepath + arguments[0] + extension;

    if (std::ifstream(filepath))
    {
        std::cout << "task already exists" << std::endl;
        return;
    }

    std::cout << "adding " << arguments[0] << " to task config" << std::endl;

    reset_task(filepath);
}

void start_handler(const std::string (&arguments)[2])
{
    // will be different on Windows (not Unix time), won't work
    // Windows time is 100ns intervals from 1/1/1601
    std::time_t unix_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::ofstream task_file;
    std::string filepath;
    filepath = config_filepath + arguments[0] + extension;

    // open file (read only) w/ position indicator at end
    std::ifstream file_check(filepath, std::ios::ate);
    std::streampos file_check_size = file_check.tellg();

    if (!file_check.is_open())
    {
        std::cout << "task does not exist" << std::endl;
        file_check.close();
        return;
    }
    if(file_check_size < 4) // should have atleast ~\n~ - 3 chars + EOF
    {
        std::cout << "task has no data, resetting config" << std::endl;
        reset_task(filepath);
    }
    else
    {
        char last_char = ' ';
        int cursor_pos = 1;
        while((last_char == '\n' || last_char == ' ') && cursor_pos < file_check_size)
        {
            file_check.seekg(-cursor_pos, std::ios::end); // set position indicator to cursor_pos behind EOF
            last_char = file_check.peek();                // get char at position indicator
            cursor_pos++;
        }
        //std::cout << "last character: " << last_char << std::endl;
        if(last_char != '~') // if final char does not equal the end of file
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
    task_file << "\n" << unix_time; // output starting time to file
    std::cout << "starting timer for " << arguments[0] << std::endl;
    std::cout << "current time: " << std::ctime(&unix_time); // ctime automatically adds newline

    task_file.close();
}

void stop_handler(const std::string (&arguments)[2])
{
    std::time_t stop_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::time_t start_time;

    std::ofstream write_file;
    std::string filepath;
    filepath = config_filepath + arguments[0] + extension;

    // open file (read only) w/ position indicator at end
    std::ifstream read_file(filepath, std::ios::ate);
    std::streampos read_file_size = read_file.tellg();
    std::string orig_file_data;

    // need to open ifstream atleast once for this - reason why using if and of instead of just fstream
    if (!read_file.is_open())
    {
        std::cout << "task does not exist" << std::endl;
        read_file.close();
        return;
    }
    if(read_file_size < 4) // should have atleast ~\n~ - 3 chars + EOF
    {
        std::cout << "task has no data" << std::endl;
        read_file.close(); // close instead of resetting task here
        return;
    }
    char last_char = ' ';
    int cursor_pos = 1;
    while((last_char == '\n' || last_char == ' ') && cursor_pos < read_file_size)
    {
        read_file.seekg(-cursor_pos, std::ios::end); // set position indicator to cursor_pos behind EOF
        last_char = read_file.peek();                // get char at position indicator
        cursor_pos++;
    }
    //std::cout << "last character: " << last_char << std::endl;
    if(last_char == '~') // if final char DOES equal the end of file
    {
        std::cout << "timer not started" << std::endl;
        read_file.close();
        return;
    }

    std::cout << "stopping timer for " << arguments[0] << std::endl;

    read_file.seekg(0, std::ios::beg);
    std::ostringstream temp_stream;
    temp_stream << read_file.rdbuf();
    orig_file_data = temp_stream.str();    

    int num_end = orig_file_data.size() - cursor_pos; 
    int num_start = orig_file_data.rfind('\n', num_end);
    std::string num_str = orig_file_data.substr(num_start, num_end - num_start + 2); // cursor_pos is off by 2

    read_file.close();

    start_time = static_cast<std::time_t>(std::stoi(num_str)); // get time_t (long int) from num_str
    int time_length = stop_time - start_time;
    std::cout << time_length / 60 << " minutes since starting" << std::endl;

    // remove start time from file string
    std::string new_file_data = orig_file_data;
    std::size_t prev_data_end = new_file_data.rfind('~', new_file_data.size() - 1);
    new_file_data.erase(new_file_data.begin() + prev_data_end, new_file_data.end()); // should remove ~

    // task entry format: start:time_since_start msg
    std::string entry_str;
    entry_str = std::to_string(start_time) + ":" + std::to_string(time_length) + " " + arguments[1]; // ADD MSG
    new_file_data.append(entry_str + "\n~"); // only need to append task entry + \n~

    write_file.open(filepath, std::ios::trunc); // remove initial data 
    if(!write_file.is_open())
    {
        std::cout << "could not open task file" << std::endl;
        write_file.close();
        return;
    }
    write_file << new_file_data;
    write_file.close();
}

void list_handler(const std::string (&arguments)[2])
{
    std::string filepath;
    filepath = config_filepath + arguments[0] + extension;
    // open file (read only) w/ position indicator at end
    std::ifstream read_file(filepath, std::ios::ate);
    std::streampos read_file_size = read_file.tellg();
    std::vector<std::string> file_data;

    if (!read_file.is_open())
    {
        std::cout << "task does not exist" << std::endl;
        read_file.close();
        return;
    }
    if(read_file_size < 4) // should have atleast ~\n~ - 3 chars + EOF
    {
        std::cout << "task has no data" << std::endl;
        read_file.close(); // close instead of resetting task here
        return;
    }

    read_file.seekg(0, std::ios::beg);  

    // get all lines into vector
    bool seenEntryChar = false;
    while(!read_file.eof())
    {
        std::string line;
        std::getline(read_file, line, '\n');

        // makes sure not to include stuff before entries
        if(!seenEntryChar)
        {
            seenEntryChar = (line[0] == '~'); 
            continue;
        }
        else if(line.empty() || line[0] == ' ' || line[0] == '\n') continue; // prevent accidental newlines from breaking program

        file_data.push_back(line);
    }

    int totalSeconds = 0;
    for(std::size_t i = 0; i < file_data.size(); i++)
    {
        std::string line = file_data[i];
        if(line[0] == '~') break; // once reached end of entries break
        task_entry entry;
        
        std::size_t time_separator = line.find_first_of(':', 0);
        std::size_t msg_separator = line.find_first_of(' ', time_separator);

        std::string start_time_str = line.substr(0, time_separator);
        std::string time_length_str = line.substr(time_separator + 1, msg_separator - time_separator);
        entry.message = line.substr(msg_separator + 1, line.length() - 1); // if no message should be ""
        entry.start_time = static_cast<std::time_t>(std::stoi(start_time_str));
        entry.time_length = std::stoi(time_length_str);

        // make border cover "message: "(9 char) + message else 50
        const int borderLength = (entry.message.length() + 9) > 50 ? (entry.message.length() + 9) : 50;
        totalSeconds += entry.time_length;
        // could figure out best duration unit to use based on time_length        
        std::cout << std::setw(borderLength) << std::setfill('=') << std::left << i+1 << "\n"; 

        if(entry.message != "" && entry.message != " ") // if message not empty
            std::cout << "message: " << entry.message << "\n";

        std::cout << std::ctime(&entry.start_time)
                  << entry.time_length / 60 << " mins" << std::endl
                  << std::setw(borderLength + 2) << std::setfill('=') << std::right << "\n\n";
                  // +2 fixes weird bug/feature where it won't fill to same length
    }
    std::cout << "TOTAL TIME: " << totalSeconds / 60 << " mins" << std::endl;
}

void init_cmd()
{
    // key of command string, value of pointer to command function
    cmap.emplace("add", &add_handler);
    cmap.emplace("start", &start_handler);
    cmap.emplace("stop", &stop_handler);
    cmap.emplace("list", &list_handler);
    config_filepath = get_config_dir();
}

void run_cmd(const std::string& command_str, const std::string (&arguments)[2])
{
    init_cmd();
    // iterator containing location of match
    std::map<std::string, cmd_handler>::const_iterator command_iter = cmap.find(command_str);
    if (command_iter == cmap.end())
    {
        std::cout << "no command matches that argument" << std::endl;
        return;
    }
    // get command handler function and run
    cmd_handler curr_cmd_handler = command_iter->second;
    curr_cmd_handler(arguments);
}