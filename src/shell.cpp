//
// Created by swaraj on 6/4/25.
//
#include <shell.h>


Shell::Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc)
        : m_p_StdPlane(p_stdPlane), m_Nc(nc) {
    m_Nc.get_term_dim(&m_DimX, &m_DimY);
    start = std::chrono::high_resolution_clock::now();

    struct ncplane_options main_plane_opts = default_main_plain_option(m_DimX, m_DimY);
    main_plane = new ncpp::Plane(*m_p_StdPlane, &main_plane_opts);
}
Shell::~Shell() {
    m_Nc.stop();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - start );
    std::cout << "Wash shell exited successfully! , it was active for " << duration << " seconds \n";

}
void Shell::status_line () {
    struct ncplane_options status_line_opts = default_status_line_option(m_DimX, m_DimY);
    p_status_line = new ncpp::Plane(m_p_StdPlane, status_line_opts);

    p_status_line->set_fg_rgb8(100,255,255);
    p_status_line->set_bg_rgb8(0,0,0);
    p_status_line->printf(0,0, "INSERT");
    m_Nc.render();

}

int Shell::ctrl_c_press_count = 0;
void Shell::handle_ctrl_c(int sig) {
    ++ctrl_c_press_count;
}


int Shell::runShell() {
    if (signal(SIGINT, Shell::handle_ctrl_c) == SIG_ERR) {
        std::cerr << "Failed to set SIGINT handler" << std::endl;
        return EXIT_FAILURE;
    }

    while (!m_Quite) {
        if (ctrl_c_press_count >= 3) break;
        main_plane->printf(m_Line, ncpp::NCAlign::Left, "%s", SHELL.c_str());
        main_plane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
        m_Nc.render();

        status_line ();
        m_Key = m_Nc.get(false, &m_NcIn);

        switch (m_Key) {
            // Exit Program when pressed ctrl-D
            case 'D':
                if (ncinput_ctrl_p(&m_NcIn)) {
                    m_Quite = true;
                }
                break;
            case NCKEY_ENTER:
                m_Line++;
                m_PromptLines.push_back(m_Prompt);
                m_Prompt.clear();
                break;
            case NCKEY_BACKSPACE:
                if (!m_Prompt.empty()) {
                    m_Prompt.pop_back();
                    // Erase old line
                    // ncplane_erase_region(*m_p_StdPlane,m_Line,-1,INT_MAX,0); // causes flickering
                    std::string clear_line(m_DimX- SHELL_x, ' ');

                    main_plane->printf(m_Line, SHELL_x, "%s", clear_line.c_str());
                    main_plane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
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
