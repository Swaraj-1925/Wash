//
// Created by swaraj on 6/4/25.
//
#include <shell.h>
#include "theme.h"


Shell::Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc,unsigned DimY,unsigned DimX)
        : m_p_StdPlane(p_stdPlane),
        m_DimY(DimY),
        m_DimX(DimX),
        m_Nc(nc)
        {

    m_Start = std::chrono::high_resolution_clock::now();
    struct ncplane_options main_plane_opts = default_main_plain_option(m_DimY, m_DimX);
    m_p_MainPlane = new ncpp::Plane(*m_p_StdPlane, &main_plane_opts);
    i_p_StatusLine = StatusLine(m_p_StdPlane,m_DimY,m_DimX);
}
Shell::~Shell() {
    m_Nc.stop();
//    delete m_p_MainPlane;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - m_Start );
    std::cout << "Wash shell exited successfully! , it was active for " << duration << " seconds \n";

}

int Shell::ctrl_c_press_count = 0;
void Shell::handle_ctrl_c(int sig) {
    ++ctrl_c_press_count;
}


static int render_status_line(struct nctablet* _t, bool cliptop __attribute__ ((unused)) ) {
    ncpp::NcTablet *Tab = ncpp::NcTablet::map_tablet(_t);
    ncpp::Plane *StatusLinePlane = Tab->get_plane();
    auto i_StatusLine = Tab->get_userptr<StatusLine>();
    StatusLinePlane->erase();

    ncpp::Cell c(' ');
    Theme theme;
    StatusLinePlane->set_fg_rgb8(theme.TEXT1.get_r(), theme.TEXT1.get_g(), theme.TEXT1.get_b());
    c.set_bg_rgb8(theme.STATUS_BG.get_r(),theme.STATUS_BG.get_g(),theme.STATUS_BG.get_b());
    StatusLinePlane->set_base_cell(c);
    StatusLinePlane->release(c);
    StatusLinePlane->set_bg_rgb(0xffffff);
    StatusLinePlane->set_fg_rgb(0x000000);
    StatusLinePlane->printf(0, 0, ncpp::NCAlign::Left, "insert mode");

    StatusLinePlane->move_top();
    return EXIT_SUCCESS;
}

int Shell::run_shell() {

    if (!m_p_MainPlane){
        m_Nc.stop();
        return EXIT_FAILURE;
    }
    Theme theme;
//    if (!m_p_MainPlane->set_bg_rgb8(theme.TERM_BG.get_r(),theme.TERM_BG.get_g(),theme.TERM_BG.get_g() )){
//        m_Nc.stop();
//        std::cerr<<"not abel to set color";
//        return EXIT_FAILURE;
//    }

    if (signal(SIGINT, Shell::handle_ctrl_c) == SIG_ERR) {
        std::cerr << "Failed to set SIGINT handler" << std::endl;
        return EXIT_FAILURE;
    }

    while (!m_Quite){
        if (ctrl_c_press_count >= 3) break;
        m_p_MainPlane->printf(m_Line, ncpp::NCAlign::Left, "%s", SHELL.c_str());
        m_p_MainPlane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
        m_Nc.render();
        m_Key = m_Nc.get(false, &m_NcIn);

        switch (m_Key) {
            // Exit Program when pressed ctrl-D
            case 'D':
                if (ncinput_ctrl_p(&m_NcIn)) {
                    m_Quite = true;
                }
                break;
            case 'A':

                if (ncinput_ctrl_p(&m_NcIn)) {
                    if (!i_p_StatusLine.m_StatusLineReel->add(nullptr, nullptr,render_status_line,&i_p_StatusLine)){
                        std::cout<<"2st if";
                        return EXIT_FAILURE;
                    }
                }

                break;
            case NCKEY_ENTER:
                m_Line++;
                m_PromptHistory.push_back(m_Prompt);
                m_Prompt.clear();
                break;
            case NCKEY_BACKSPACE:
                if (!m_Prompt.empty()) {
                    m_Prompt.pop_back();
                    // Erase old line
                    // ncplane_erase_region(*m_p_StdPlane,m_Line,-1,INT_MAX,0); // causes flickering
                std::string clear_line(m_DimY- SHELL_x, ' ');

                    m_p_MainPlane->printf(m_Line, SHELL_x, "%s", clear_line.c_str());
                    m_p_MainPlane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
                    m_Nc.render();
                    break;
                }
            default:
                if (m_Key >= 32 && m_Key <= 126) {
                    m_Prompt += static_cast<char>(m_Key);
                } else if (m_Key >= 0x80 && m_Key <= 0x10FFFF) {
                    unsigned char utf8_buf[5] = {0};
                    if (ncpp::NotCurses::ucs32_to_utf8(&m_Key, 1, utf8_buf, sizeof(utf8_buf))) {
                        m_Prompt.append(reinterpret_cast<char *>(utf8_buf));
                    }
                }
        }

    }
    return EXIT_SUCCESS;
}
