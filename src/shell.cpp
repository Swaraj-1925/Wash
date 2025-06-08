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
    Shell::create_tab();
    i_p_StatusLine = StatusLine(m_p_StdPlane,m_DimY,m_DimX);
    i_p_StatusLine.render_status_line(m_Tabs);
}
Shell::~Shell() {
    m_Nc.stop();
//    delete m_p_Tab;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - m_Start );
    std::cout << "Wash shell exited successfully! , it was active for " << duration << " seconds \n";

}

int Shell::ctrl_c_press_count = 0;
void Shell::handle_ctrl_c(int sig) {
    ++ctrl_c_press_count;
}



int Shell::create_tab(){
    std::string tab_name = "Tab " + std::to_string(m_Tabs.size() + 1);

    for (auto &tab : m_Tabs) {
        tab.active = false;
    }

    struct ncplane_options TabOpts = default_tab_option(m_DimY, m_DimX);
    ncpp::Plane *m_p_Tab = new ncpp::Plane(*m_p_StdPlane, &TabOpts);

    m_p_Tab->set_scrolling(true);

    ncpp::Cell base(' ');
    base.set_bg_rgb8(t.TERM_BASE_BG.get_r(),
                     t.TERM_BASE_BG.get_g(),
                     t.TERM_BASE_BG.get_b());

    base.set_fg_rgb8(t.TEXT0.get_r(),
                     t.TEXT0.get_g(),
                     t.TEXT0.get_b());
    m_p_Tab->set_base_cell(base);
    m_p_Tab->erase();

    m_Tab =  Tab{
            m_p_Tab,
            tab_name,
            M_VISUAL,
            {},
            true
    };
    m_Tabs.push_back(m_Tab);

    return EXIT_SUCCESS;
}

 struct Tab Shell::get_active_tab() {
    struct Tab tab;
    for(auto const &it: m_Tabs){
        if (it.active){
            tab = it;
            break;
        }
    }
     return tab;
}

int Shell::run_shell() {

    ncpp::Plane *CurrTab = m_Tab.plane;

    if (!CurrTab){
        m_Nc.stop();
        std::cout<<"Not abel to find active tab";
        return EXIT_FAILURE;
    }

    if (signal(SIGINT, Shell::handle_ctrl_c) == SIG_ERR) {
        std::cerr << "Failed to set SIGINT handler" << std::endl;
        return EXIT_FAILURE;
    }
    m_Nc.render();
    while (!m_Quite){
        if (ctrl_c_press_count >= 3) break;
        CurrTab->printf(m_Line, ncpp::NCAlign::Left, "%s", SHELL.c_str());
        CurrTab->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
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
                m_Tab.PromptHistory.push_back(m_Prompt);
                m_Prompt.clear();
                break;
            case NCKEY_BACKSPACE:
                if (!m_Prompt.empty()) {
                    m_Prompt.pop_back();
                    // Erase old line
                     ncplane_erase_region(*CurrTab, m_Line, -1, INT_MAX, 0); // causes flickering
//                std::string clear_line(m_DimY- SHELL_x, ' ');
//
//                    m_p_Tab->printf(m_Line, SHELL_x, "%s", clear_line.c_str());
//                    m_p_Tab->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
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
                break;
        }
        m_Nc.render();

    }
    return EXIT_SUCCESS;
}
