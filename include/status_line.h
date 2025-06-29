//
// Created by swaraj on 6/5/25.
//

#ifndef WASH_STATUS_LINE_H
#define WASH_STATUS_LINE_H

#include <vector>
#include <iostream>
#include <string>
#include <ncpp/NotCurses.hh>
#include <ncpp/Reel.hh>

#include "options.h"
#include "theme.h"
#include "shortcut.h"
#include "tab.h"

class StatusLine{
private:
    ncpp::Plane *m_p_StdPlane;
    ncpp::Plane *m_p_StatusLinePlane;
    ncpp::Plane *m_p_ModePlane;

    unsigned m_DimX = 0,m_DimY = 0;
    Theme theme;

    std::string get_mode();

    std::vector<std::string> m_promote_history;

    bool m_Quite = false;

    uint32_t m_Key;
    ncinput m_NcIn;


    std::string m_Command;
    std::vector<std::string> m_CommandHistory;
public:
    bool m_Status = false;
public:
    StatusLine() = default;
    StatusLine(ncpp::Plane *std_plane, unsigned &dim_y, unsigned &dim_x);
    ~StatusLine() = default;

    int render_status_line(std::vector<Tab> &tabs);
    int status_line_command(ncpp::NotCurses* nc);
    void toggle_status();

};


#endif //WASH_STATUS_LINE_H


