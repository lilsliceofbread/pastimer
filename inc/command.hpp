#pragma once

enum CommandType {
    ADD,
    START,
    STOP,
    GET_TIME
};

void initCmd();
CommandType compareCmd(const std::string& command_str);
void runCmd(CommandType command, const std::string& argument);