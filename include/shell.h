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
class Shell {
public:
    Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc,unsigned DimY,unsigned DimX);
    ~Shell();

    int run_shell();

private:
    static void handle_ctrl_c(int sig);

    ncpp::Plane* m_p_StdPlane;
    ncpp::Plane* m_p_MainPlane;
    ncpp::Plane *m_p_StatusLinePlane;


    std::unique_ptr<ncpp::NcReel> m_StatusLineReel;

    std::chrono::time_point<std::chrono::system_clock> m_Start;
    ncpp::NotCurses &m_Nc;
    ncinput m_NcIn;

    unsigned m_DimY, m_DimX;
    bool m_Quite = false;
    uint32_t m_Key;

    int m_Line = 0;
    std::string m_Prompt;
    std::vector<std::string> m_PromptHistory;
    const std::string SHELL = "[WASH]~ ";
    const int SHELL_x = SHELL.length();

    static int ctrl_c_press_count ;

    StatusLine i_p_StatusLine ;
};
#endif //WASH_SHELL_H
