//
// Created by swaraj on 6/5/25.
//

#include "../include/status_line.h"

StatusLine::StatusLine(ncpp::Plane *std_plane,unsigned &dim_y,unsigned &dim_x)
:m_p_StdPlane(std_plane),m_DimY(dim_y), m_DimX(dim_x)
{
    struct ncplane_options status_line_opts = default_statusLine_option(m_DimY, m_DimX);
    StatusLine::m_p_StatusLinePlane = new ncpp::Plane(m_p_StdPlane, status_line_opts);
    ncpp::Cell base(' ');
    base.set_bg_rgb8(theme.TERM_MANTLE_BG.get_r(),
                     theme.TERM_MANTLE_BG.get_g(),
                     theme.TERM_MANTLE_BG.get_b());
    base.set_fg_rgb8(theme.TEXT1.get_r(),
                     theme.TEXT1.get_g(),
                     theme.TEXT1.get_b());

    m_p_StatusLinePlane->set_base_cell(base);
    m_p_StatusLinePlane->erase();
    m_p_StatusLinePlane->release(base);

}

StatusLine::StatusLine() {}
StatusLine::~StatusLine() {
//    delete m_StatusLineReel;
}

int StatusLine::render_status_line(std::vector<Tab> &tabs) {
    m_p_StatusLinePlane->erase();
    int col = 0;  // X position to start printing from

    for (const auto &t : tabs) {
        std::string label = " " + t.name + " ";

        if (t.active) {
            // Optional: highlight active tab
            m_p_StatusLinePlane->set_fg_rgb8(255, 255, 255);  // White text
            m_p_StatusLinePlane->set_bg_rgb8(0, 128, 255);    // Blue background
        } else {
            // Normal tab style
            m_p_StatusLinePlane->set_fg_rgb8(180, 180, 180);  // Gray text
            m_p_StatusLinePlane->set_bg_rgb8(50, 50, 50);     // Dark background
        }

        m_p_StatusLinePlane->printf(0, col, "%s", label.c_str());
        col += label.length();  // Move X position forward
    }

    m_p_StatusLinePlane->set_fg_default();
    m_p_StatusLinePlane->set_bg_default();

    return EXIT_SUCCESS;

}