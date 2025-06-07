//
// Created by swaraj on 6/5/25.
//

#ifndef WASH_STATUS_LINE_H
#define WASH_STATUS_LINE_H

#include <vector>
#include <iostream>
#include <string>
#include <ncpp/NotCurses.hh>

#include "options.h"
#include "theme.h"

// Terminal Modes
enum T_Mode {
    M_INSERT,
    M_COMMAND,
    M_VISUAL
};

class StatusLine{
private:
    std::vector<std::string> m_tabs;
    int m_ActiveTabIdx =0;

    ncpp::Plane *m_p_StdPlane;
    ncpp::Plane *m_p_StatusLinePlane;
//    ncpp::NcReel m_p_StatusLineReel;

    unsigned m_DimX,m_DimY;
    Theme theme;
    int add_tab(const std::string &title);

public:
    StatusLine(ncpp::Plane *std_plane,unsigned &dim_x,unsigned &dim_y);
    ~StatusLine();

    enum T_Mode m_mode = M_VISUAL;
    std::vector<std::string> m_promote_history;

    int render_status_line();
};


#endif //WASH_STATUS_LINE_H


