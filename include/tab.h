//
// Created by swaraj on 6/8/25.
//

#ifndef WASH_TAB_H
#define WASH_TAB_H
#include <iostream>
#include <string>
#include <vector>
#include <ncpp/NotCurses.hh>
#include <pwd.h>
#include <unistd.h>

#include "theme.h"
#include "options.h"
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

private:
    ncpp::Plane *m_p_StdPlane = nullptr;
    ncpp::Plane* m_p_Plane = nullptr;

    std::string m_HomeDir;
    std::string m_Username;
    std::string m_HostName;

    Theme t;


public:
    Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, int tab_index);
    Tab() = default;
    ~Tab() = default;

    void update_current_path();
    void handle_enter_press();
    void handle_backspace_press(int dim_x);
    int handle_default(uint32_t m_Key);

    void handle_prompt();

private:
    void render_prompt(){}
};
#endif //WASH_TAB_H
