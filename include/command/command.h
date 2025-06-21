//
// Created by swaraj on 6/13/25.
//

#ifndef WASH_COMMAND_H
#define WASH_COMMAND_H
#include <iostream>
#include <vector>
#include <ncpp/NotCurses.hh>
#include <pwd.h>
#include <unistd.h>

class Command {
public:
    virtual ~Command() = default;
    virtual std::string  execute(const std::vector<std::string>& args) = 0;
//    virtual std::vector<std::string> parse_args(const std::vector<std::string>& args) = 0;
    virtual void parse_args(const std::vector<std::string>& args) = 0;
};




#endif //WASH_COMMAND_H
