//
// Created by swaraj on 6/5/25.
//

#include "../include/status_line.h"

StatusLine::StatusLine(ncpp::Plane *std_plane,unsigned &dim_y,unsigned &dim_x)
:m_p_StdPlane(std_plane)
{
    StatusLine::m_p_StdPlane = std_plane;
    StatusLine::m_DimY = dim_y;
    StatusLine::m_DimX = dim_x;

    m_tabs.push_back("a");
    m_tabs.push_back("");

    struct ncplane_options status_line_opts = default_status_line_option(m_DimY, m_DimX);
    StatusLine::m_p_StatusLinePlane = new ncpp::Plane(m_p_StdPlane, status_line_opts);

    struct ncreel_options ReelOption = default_status_line_reel_option();

    m_p_StatusLinePlane->set_userptr(m_StatusLineReel);
    m_StatusLineReel = new ncpp::NcReel(m_p_StatusLinePlane,&ReelOption);

}
StatusLine::StatusLine() {}
StatusLine::~StatusLine() {
//    delete m_StatusLineReel;
}

int StatusLine::add_tab(const std::string &title) {
    m_tabs.push_back(title);
    // if you want the newly added tab to become active:
    m_ActiveTabIdx = int(m_tabs.size()) - 1;
    return m_ActiveTabIdx;
}
