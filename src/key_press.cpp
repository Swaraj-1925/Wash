//
// Created by swaraj on 6/15/25.
//

#include "tab.h"

void Tab::handle_enter_press() {
    Tab::m_CommandHistory.push_back(Tab::m_Command);
    if(m_Command == "clear"){
        Tab::m_p_Plane->erase();
        Tab::m_Command.clear();
        Tab::m_Line = 0;
    } else{
        Tab::m_Line ++;
        if(Tab::parse_and_execute_command(m_Command) == EXIT_FAILURE) {
            Tab::m_p_Plane->printf(Tab::m_Line++,NCALIGN_CENTER,"Failed to executing command : %s",m_Command.c_str());
        }
        Tab::m_Command.clear();
    }
    Tab::update_current_path();
    Tab::m_CommandIdx += 1;
}

int Tab::handle_default(uint32_t m_Key) {
    int cursor_x = (int) Tab::m_ShellLen + Tab::m_Command.length();
    Tab::m_p_Plane->cursor_move(Tab::m_Line, cursor_x);

    if (m_Key >= 32 && m_Key <= 126) {
        Tab::m_Command += static_cast<char>(m_Key);
    } else if (m_Key >= 0x80 && m_Key <= 0x10FFFF) {
        unsigned char utf8_buf[5] = {0};
        if (ncpp::NotCurses::ucs32_to_utf8(&m_Key, 1, utf8_buf, sizeof(utf8_buf))) {
            Tab::m_Command.append(reinterpret_cast<char *>(utf8_buf));
        }
    }
    return cursor_x;
}
void  Tab::handle_backspace_press(int dim_x) {
    if (m_Command.empty()) return;
    Tab::m_Command.pop_back();
    // start from row, start col,num of row to erase,erase col location
    ncplane_erase_region(*m_p_Plane, m_Line, m_ShellLen, 1, dim_x - m_ShellLen); // causes flickering

}