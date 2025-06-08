//
// Created by swaraj on 6/8/25.
//

#ifndef WASH_TAB_H
#define WASH_TAB_H
#include <iostream>
#include <string>
#include <vector>
#include <ncpp/NotCurses.hh>
// Terminal Modes
enum T_Mode {
    M_INSERT,
    M_COMMAND,
    M_VISUAL
};

struct Tab {
    ncpp::Plane* plane;
    std::string  name;
    enum T_Mode mode = M_VISUAL;
    std::vector<std::string> PromptHistory;
    bool active = false;

};
#endif //WASH_TAB_H
