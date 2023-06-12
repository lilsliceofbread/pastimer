#include <iostream>
#include <command.hpp>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "no arguments given" << std::endl;
        return 0;
    }

    std::string command_str;
    std::string argument_str;
        
    command_str = argv[1];
    if(argc  >= 3) //don't read past end of array
        argument_str = argv[2];

    initCmd();

    //epic title for no reason whatsoever
    std::cout << 
    " _____         _   _                \n"               
    "|  _  |___ ___| |_|_|_____ ___ ___  \n"
    "|   __| .'|_ -|  _| |     | -_|  _| \n"
    "|__|  |__,|___|_| |_|_|_|_|___|_|   \n" << std::endl;

    CommandType command = compareCmd(command_str);
    
    runCmd(command, argument_str);

    return 0;
}