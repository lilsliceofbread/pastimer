#pragma once

enum CommandType {
    ADD,
    START,
    STOP,
    GET_TIME
};

void initCmd();
CommandType compareCmd(std::string command_str);