//
// Created by swaraj on 6/8/25.
//

#ifndef WASH_TAB_H
#define WASH_TAB_H
#include <iostream>
#include <string>
#include <vector>
#include <ncpp/NotCurses.hh>
#include <unordered_map>
#include <sstream>

#include "theme.h"
#include "options.h"
#include "command/command.h"
// Terminal Modes
enum T_Mode {
    M_INSERT,
    M_COMMAND,
    M_VISUAL
};

class Tab {
public:
    std::string  m_Name;

    enum T_Mode m_Mode = M_VISUAL;
    bool m_Active = true;

    int m_Line = 0;
    int m_ShellLen = 0;
    std::string m_SHELL;

    std::string m_Command;
    std::vector<std::string> m_CommandHistory;
    std::string m_CurrentPath;

    ncpp::Plane* m_p_Plane = nullptr;
private:
    ncpp::Plane *m_p_StdPlane = nullptr;

    std::string m_HomeDir;
    std::string m_Username;
    std::string m_HostName;

    Theme t;
    std::unordered_map<std::string, std::shared_ptr<Command>> command_map;

public:
    Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, std::string name);
    Tab() = default;
    ~Tab() = default;

    void update_current_path();
    void handle_enter_press();
    void handle_backspace_press(int dim_x);
    int handle_default(uint32_t m_Key);

    void handle_prompt();
    std::string get_mode();

private:
    int parse_and_execute_command(const std::string& line);
    void register_builtin_commands();
};
#endif //WASH_TAB_H
