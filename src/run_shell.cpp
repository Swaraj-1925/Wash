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
        Tab &curTab = m_Tabs[m_TabIdx];
        if (ctrl_c_press_count >= 3) break;
        m_Key = m_Nc.get(false, &m_NcIn);

        curTab.handle_prompt();

        if(WS_QUIT){
            m_Quite = true;
        } else if(WS_ENTER){
            curTab.handle_enter_press();
            continue;
        }else if(WS_BACKSPACE && !curTab.m_Command.empty()){
            curTab.handle_backspace_press(m_DimX);
            m_Nc.render();
        } else if(WS_TOGGLE_STATUS_LINE){
            i_p_StatusLine.toggle_status();

        } else if( i_p_StatusLine.m_Status && WS_NEW_TAB){
            i_p_StatusLine.toggle_status();
            create_tab();
        }else if( i_p_StatusLine.m_Status && WS_STATUS_LINE_MCOMMAND) {
            i_p_StatusLine.toggle_status();
            m_Nc.cursor_disable();
            i_p_StatusLine.status_line_command(&m_Nc);

        }
        else if( i_p_StatusLine.m_Status && WS_MOVE_LEFT_TAB){

            if (m_Tabs.size() <= 1) {
                i_p_StatusLine.toggle_status();
                continue;
            }
            m_Tabs[m_TabIdx].m_Active = false;

            //if index is less than or equal to 0 go to last tab
            if (m_TabIdx <= 0) {
                m_TabIdx = (int)m_Tabs.size() - 1;
            } else {
                m_TabIdx--;
            }
            m_Tabs[m_TabIdx].m_Active = true;
            m_Tabs[m_TabIdx].m_p_Plane->move_top();
            i_p_StatusLine.toggle_status();
            m_Nc.render();
        }
        else if( i_p_StatusLine.m_Status && WS_MOVE_RIGHT_TAB){
            if (m_Tabs.size() <= 1) {
                i_p_StatusLine.toggle_status();
                continue;
            }
            // if index greater than tab size go to 1st tab
            m_Tabs[m_TabIdx].m_Active = false;
            if (m_TabIdx >= (int)m_Tabs.size() - 1) {
                m_TabIdx = 0;
            } else {
                m_TabIdx++;
            }

            m_Tabs[m_TabIdx].m_Active = true;
            m_Tabs[m_TabIdx].m_p_Plane->move_top();
            i_p_StatusLine.toggle_status();
            m_Nc.render();
        }
        else{
            int cursor_x = curTab.handle_default(m_Key);
            m_Nc.cursor_enable(curTab.m_Line, cursor_x);
        }
        i_p_StatusLine.render_status_line(m_Tabs);
        m_Nc.render();
    }
    return EXIT_SUCCESS;
}
