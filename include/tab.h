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
#include <pty.h>
#include <utmp.h>

#include "theme.h"
#include "options.h"
#include "command/command.h"
#include "constants.h"
#include "auto_completion.h"
// Terminal Modes
enum T_Mode {
    M_INSERT,
    M_COMMAND,
    M_VISUAL
};

class Tab {
public:
    std::string  m_Name;
    ncpp::Plane *m_p_NotificationPlane;

    T_Mode m_Mode {M_INSERT};
    bool m_Active = true;

    int m_Line = 0;
    int m_ShellLen = 0;
    int m_CursorIdx = 0;
    int m_CommandIdx = 0;
    std::string m_SHELL;

    std::string m_Command;
    std::vector<std::string> m_CommandHistory;
    std::string m_CurrentPath;

    ncpp::Plane* m_p_Plane = nullptr;
    std::vector<std::string > errors;
    std::vector<std::string > debug;

private:
    ncpp::Plane *m_p_StdPlane = nullptr;
    Output m_output;


    std::string m_HomeDir;
    std::string m_Username;
    std::string m_HostName;

    Theme t;
    std::unordered_map<std::string, std::shared_ptr<Command>> command_map;

    unsigned DimY,DimX;
    std::string escape_string(const std::string& input) {
        std::string result;
        for (char c : input) {
            switch (c) {
                case '\n': result += "\\n"; break;
                case '\t': result += "\\t"; break;
                case '\r': result += "\\r"; break;
                default: result += c; break;
            }
        }
        return result;
    }

public:
    Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, std::string name);
    Tab() = default;
    ~Tab();

    void update_current_path();
    void handle_enter_press();
    void handle_backspace_press(int dim_x);
    int handle_default(uint32_t m_Key);

    void handle_prompt();
    void handle_tab();
    std::string get_mode();
    // Check and update background process statuses

private:
    std::vector<char*> parse_command(const std::string& line);
    int handle_command(std::vector<char*>& exec_args);
    int execute_command(pid_t fpid ,int *filedes,std::vector<char *> &exec_args);
    void free_args(std::vector<char*>& args);
    void register_builtin_commands();
    void get_output();
};
#endif //WASH_TAB_H
