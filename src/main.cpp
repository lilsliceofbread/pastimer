#include <iostream>
#include <command.hpp>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "no arguments given" << std::endl;
        return 0;
    }

    std::string command_str;
    command_str = argv[1];

    initCmd();

    CommandType command = compareCmd(command_str);
    switch(command) {
        case ADD:
            std::cout << "adding " << std::endl;
            break;
        case START:
            std::cout << "starting " << std::endl;
            break;
        case STOP:
            std::cout << "stopping " << std::endl;
            break;
        case GET_TIME:
            std::cout << "time is" << std::endl;
            break;
    }

    return 0;
}