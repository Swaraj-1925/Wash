//
// Created by swaraj on 6/5/25.
//

#ifndef WASH_SHELL_H
#define WASH_SHELL_H

#include <iostream>
#include <ncpp/NotCurses.hh>
#include <vector>
#include <chrono>

#include "options.h"
#include "status_line.h"
#include "tab.h"
#include "theme.h"
#include "shortcut.h"

class Shell {
public:
    Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc,unsigned DimY,unsigned DimX);
    ~Shell();


    std::vector<Tab> m_Tabs;
    int m_TabIdx=0;


public:
    int run_shell();

private:

    std::chrono::time_point<std::chrono::system_clock> m_Start;

    ncpp::Plane *m_p_StdPlane;
    ncpp::NotCurses &m_Nc;
    ncinput m_NcIn;

    unsigned m_DimY, m_DimX;
    bool m_Quite = false;
    uint32_t m_Key;

    static int ctrl_c_press_count ;
    StatusLine i_p_StatusLine ;



private:
    static void handle_ctrl_c(int sig);
    int create_tab();
};
#endif //WASH_SHELL_H
