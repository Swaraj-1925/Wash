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

    unsigned m_DimX,m_DimY;
    Theme theme;

    std::string get_mode();

    std::vector<std::string> m_promote_history;
public:
    StatusLine();
    StatusLine(ncpp::Plane *std_plane, unsigned &dim_y, unsigned &dim_x);
    ~StatusLine();

    int render_status_line(std::vector<Tab> &tabs);

};


#endif //WASH_STATUS_LINE_H


