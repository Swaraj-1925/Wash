//
// Created by swaraj on 6/4/25.
//
#include <ncpp/NotCurses.hh>
#include <ncpp/NCKey.hh>
#include <csignal>

#include "utils/constants.h"
#include "utils/helper.h"

// TODO: create a Shell class , wichh will take these two values as an input in constructor, this class will Iheriated from other interface class which will have all the shell function
// - a public main loop function
// - private function for switch cases
// - delete helper file move logic in the class

class Shell {
public:
    Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc)
            : m_p_StdPlane(p_stdPlane), m_Nc(nc) {
        m_Nc.get_term_dim(&m_DimX,&m_DimY);
        start = std::chrono::high_resolution_clock::now();
    }
    ~Shell() {
        m_Nc.stop();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - start );
        std::cout << "Wash shell exited successfully! , it was active for " << duration << " seconds \n";

    }

private:
    std::chrono::time_point<std::chrono::system_clock> start;
    ncpp::Plane *m_p_StdPlane;
    ncpp::NotCurses &m_Nc;
    ncinput m_NcIn;

    unsigned m_DimY , m_DimX;
    bool m_Quite = false;
    uint32_t m_Key;

    int m_Line;
    std::string m_Prompt;
    std::vector<std::string> m_PromptLines;


public:
    int runShell() {
        if (signal(SIGINT, handle_ctrl_c) == SIG_ERR) {
            std::cerr << "Failed to set SIGINT handler" << std::endl;
            return EXIT_FAILURE;
        }

        while (!m_Quite) {
            if (ctrl_c_press_count >= 3) break;
            m_p_StdPlane->printf(m_Line, ncpp::NCAlign::Left, "%s", SHELL.c_str());
            m_p_StdPlane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
            m_Nc.render();

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

                        m_p_StdPlane->printf(m_Line, SHELL_x, "%s", clear_line.c_str());
                        m_p_StdPlane->printf(m_Line, SHELL_x, "%s", m_Prompt.c_str());
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
};