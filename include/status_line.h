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
#include "tab.h"

class StatusLine{
private:
    ncpp::Plane *m_p_StdPlane;
    ncpp::Plane *m_p_StatusLinePlane;

    unsigned m_DimX = 0,m_DimY = 0;
    Theme theme;

    std::string get_mode();

    std::vector<std::string> m_promote_history;
public:
    bool m_Status = false;
public:
    StatusLine() = default;
    StatusLine(ncpp::Plane *std_plane, unsigned &dim_y, unsigned &dim_x);
    ~StatusLine() = default;

    int render_status_line(std::vector<Tab> &tabs);
    void clear_status_line(ncpp::NotCurses* nc);
    void toggle_status();


};


#endif //WASH_STATUS_LINE_H


