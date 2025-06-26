//
// Created by swaraj on 6/8/25.
//

#ifndef WASH_TAB_H
#define WASH_TAB_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <ncpp/NotCurses.hh>
#include <fstream>
#include <iterator>

#include "theme.h"
#include "options.h"
#include "command/command.h"
#include "constants.h"
#include "w_pwd.h"
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
    int m_CommandIdx = 0;
    std::string m_CurrentPath;

    ncpp::Plane *m_p_OutputPlane = nullptr;
    ncpp::Plane* m_p_Plane = nullptr;
private:
    ncpp::Plane *m_p_StdPlane = nullptr;
    Output m_output;

    std::string m_HomeDir;
    std::string m_Username;
    std::string m_HostName;

    Theme t;
    std::unordered_map<std::string, std::shared_ptr<Command>> command_map;
    unsigned DimY,DimX;

    std::unordered_set<std::string> COMMAND_WITH_OUTPUT = {
            "ls",
            "echo",
            "cat",
            "mkdir",
            "pwd",
    };

    std::vector<std::string > errors;
public:
    Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, std::string name);
    Tab() = default;
    ~Tab();

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
