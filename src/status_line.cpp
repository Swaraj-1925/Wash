//
// Created by swaraj on 6/5/25.
//

#include "status_line.h"


// TODO: switch between Tab view -> Command mode
// TODO: Rename Tabs
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
//    m_p_StatusLinePlane->release(base);
}


int StatusLine::render_status_line(std::vector<Tab> &tabs) {
    m_p_StatusLinePlane->erase();
    int col = 0;  // X position to start printing from
    for (const auto &t : tabs) {
        std::string label = " " + t.m_Name+ " ";

        if (t.m_Active) {
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

    m_p_StatusLinePlane->move_top();
    m_p_StatusLinePlane->set_fg_default();
    m_p_StatusLinePlane->set_bg_default();


    return EXIT_SUCCESS;

}
void StatusLine::clear_status_line(ncpp::NotCurses* nc) {
    std::string  prompt_prefix = ":";
    if (m_p_StatusLinePlane) {
        m_p_StatusLinePlane->erase();
        // Render any static parts of status line if needed, e.g. mode indicator
        // Then draw the prompt prefix at column 0:
        m_p_StatusLinePlane->printf(0, ncpp::NCAlign::Left, "%s", prompt_prefix.c_str());
        // Move cursor to after prefix:
        unsigned x = prompt_prefix.length();
        m_p_StatusLinePlane->cursor_move(0, m_DimX);

        ncreader_options ropts = {
//                .tchannels = NCCHANNELS_INITIALIZER(0x00ffffff, 0), // white fg on black
                .tattrword = NCSTYLE_BOLD,
                .flags = NCREADER_OPTION_CURSOR | NCREADER_OPTION_HORSCROLL
            };
        ncreader *reader = ncreader_create(m_p_StatusLinePlane->to_ncplane(),&ropts);

        uint32_t m_Key;
        ncinput m_NcIn;
        while (1){
            m_Key = nc->get(true,&m_NcIn);
            m_p_StatusLinePlane->printf(0,NCALIGN_LEFT,"%d",m_Key);
        }
    }
}
void StatusLine::toggle_status() {
    StatusLine::m_Status = !StatusLine::m_Status;
}