//
// Created by swaraj on 6/15/25.
//
#include "shell.h"

int Shell::run_shell() {


    if (signal(SIGINT, Shell::handle_ctrl_c) == SIG_ERR) {
        std::cerr << "Failed to set SIGINT handler" << std::endl;
        return EXIT_FAILURE;
    }

    m_Nc.render();

    while (!m_Quite){
        if (ctrl_c_press_count >= 3) break;
        m_Key = m_Nc.get(false, &m_NcIn);

        m_Tab.handle_prompt();

        if(WS_QUIT){
            m_Quite = true;
        } else if(WS_ENTER){
            m_Tab.handle_enter_press();
            continue;
        }else if(WS_BACKSPACE){
            m_Tab.handle_backspace_press(m_DimX);
            m_Nc.render();
        } else if(WS_TOGGLE_STATUS_LINE){
            i_p_StatusLine.toggle_status();
        } else if( i_p_StatusLine.m_Status && WS_NEW_TAB){
            i_p_StatusLine.toggle_status();
            create_tab();

        }else if( i_p_StatusLine.m_Status && WS_STATUS_LINE_MCOMMAND){
            i_p_StatusLine.toggle_status();
            i_p_StatusLine.clear_status_line(&m_Nc);
        }
        else{
            int cursor_x = m_Tab.handle_default(m_Key);
            m_Nc.cursor_enable(m_Tab.m_Line, cursor_x);
        }
        i_p_StatusLine.render_status_line(m_Tabs);
        m_Nc.render();
    }
    return EXIT_SUCCESS;
}
