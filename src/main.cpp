#include <iostream>
#include <command.hpp>

int main(int argc, char *argv[])
{
    // epic title for no reason whatsoever
    std::cout << " _____         _   _                \n"
                 "|  _  |___ ___| |_|_|_____ ___ ___  \n"
                 "|   __| .'|_ -|  _| |     | -_|  _| \n"
                 "|__|  |__,|___|_| |_|_|_|_|___|_|   \n"
              << std::endl;

    if (argc < 2)
    {
        std::cout << "no arguments given" << std::endl;
        return 0;
    }

    // could do args[] = argv[1] to argv[argc - 1] for multiple args
    // use vector instead
    std::string command_str;
    std::string argument_strs[2];
    switch(argc) {
        case 0:
        case 1:
        case 2:
            std::cout << "not enough arguments given" << std::endl;
            break;
        case 3:
            command_str = argv[1];
            argument_strs[0] = argv[2];
            argument_strs[1] = "";

            run_cmd(command_str, argument_strs);
            break;
        default: // if too many arguments just ignore the rest 
            command_str = argv[1];
            argument_strs[0] = argv[2];
            argument_strs[1] = argv[3];

            run_cmd(command_str, argument_strs);
            break;
    }

    return 0;
}