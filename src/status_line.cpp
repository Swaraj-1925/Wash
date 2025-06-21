//
// Created by swaraj on 6/5/25.
//

#include <thread>
#include "status_line.h"


// TODO: switch between Tab view -> Command mode
// TODO: Rename Tabs
StatusLine::StatusLine(ncpp::Plane *std_plane,unsigned &dim_y,unsigned &dim_x)
:m_p_StdPlane(std_plane),m_DimY(dim_y), m_DimX(dim_x)
{
    struct ncplane_options status_line_opts = default_statusLine_option(m_DimY, m_DimX);
    struct ncplane_options mode_opts = default_mode_option(m_DimY, m_DimX);

    StatusLine::m_p_StatusLinePlane = new ncpp::Plane(m_p_StdPlane, status_line_opts);
    StatusLine::m_p_ModePlane = new ncpp::Plane(m_p_StdPlane, mode_opts);

    ncpp::Cell base(' ');
    base.set_bg_rgb8(theme.TERM_MANTLE_BG.get_r(),
                     theme.TERM_MANTLE_BG.get_g(),
                     theme.TERM_MANTLE_BG.get_b());
    base.set_fg_rgb8(theme.SUB_TEXT1.get_r(),
                     theme.SUB_TEXT1.get_g(),
                     theme.SUB_TEXT1.get_b());

    m_p_StatusLinePlane->set_base_cell(base);
    m_p_ModePlane->set_base_cell(base);
    m_p_StatusLinePlane->erase();
    m_p_StatusLinePlane->release(base);
}


int StatusLine::render_status_line(std::vector<Tab> &tabs) {
    m_p_StatusLinePlane->erase();
    std::string mode;
    int col = 10;  // X position to start printing from
    for (auto &t : tabs) {
        std::string label = " " + t.m_Name+ " ";

        if (t.m_Active) {
            mode = t.get_mode();
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
    m_p_ModePlane->printf(0, 0, "%s", mode.c_str());
    m_p_StatusLinePlane->move_top();
    m_p_ModePlane->move_top();
    m_p_StatusLinePlane->set_fg_default();
    m_p_StatusLinePlane->set_bg_default();


    return EXIT_SUCCESS;

}
int StatusLine::status_line_command(ncpp::NotCurses* nc) {
    if (!m_p_StatusLinePlane) return EXIT_FAILURE;
    m_p_StatusLinePlane->erase();

    std::string prompt_prefix = "COMMAND : ";

    unsigned promptLen = prompt_prefix.length();
    unsigned row, col;
    m_p_StatusLinePlane->get_dim(&row, &col);
    nc->cursor_enable(col, row);

    m_p_ModePlane->erase();
    m_p_ModePlane->printf(0, 0, "%s", prompt_prefix.c_str());
    m_p_StatusLinePlane->cursor_move(0, m_DimX);

    nc->render();
    while (!m_Quite) {
        m_Key = nc->get(false, &m_NcIn);
        if (WS_QUIT) {
            m_Quite = true;
        } else if (WS_ENTER) {
            m_CommandHistory.push_back(m_Command);
            m_Command.clear();
            break;
        } else {
            int cursor_x = (int) promptLen + m_Command.length();
            m_p_StatusLinePlane->cursor_move(0, cursor_x);

            if (m_Key >= 32 && m_Key <= 126) {
                m_Command += static_cast<char>(m_Key);
            } else if (m_Key >= 0x80 && m_Key <= 0x10FFFF) {
                unsigned char utf8_buf[5] = {0};
                if (ncpp::NotCurses::ucs32_to_utf8(&m_Key, 1, utf8_buf, sizeof(utf8_buf))) {
                    m_Command.append(reinterpret_cast<char *>(utf8_buf));
                }
            }
            m_p_StatusLinePlane->printf(0, promptLen, "%s", m_Command.c_str());
            nc->cursor_enable(col, cursor_x);
            nc->render();
        }
    }
    return EXIT_SUCCESS;
}
void StatusLine::toggle_status() {
    StatusLine::m_Status = !StatusLine::m_Status;
}