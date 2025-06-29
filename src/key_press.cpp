//
// Created by swaraj on 6/15/25.
//

#include "tab.h"

void Tab::handle_enter_press() {
    if(m_Command.empty()){
        Tab::m_Line +=1;
        Tab::m_Command.clear();
//        Tab::free_args(args);
        Tab::m_CursorIdx = 0;
        Tab::update_current_path();
        Tab::m_CommandIdx += 1;
        return;
    }
    std::vector<char*> args = Tab::parse_command(m_Command);
    Tab::m_CommandHistory.push_back(Tab::m_Command);
//    for (const auto* arg : args) {
//        debug.push_back(std::string("debug args cd : ") + (arg ? arg : "nullptr") + "\n");
//    }
    if(strcmp(args[0] ,"clear") == 0){
        Tab::m_p_Plane->erase();
        Tab::m_Line = 0;
    } else if(strcmp(args[0] ,"exit") == 0){
        int exit_code = 0;
        if(args[1] != nullptr){
            exit_code = strtol(args[1], nullptr,10);
        }
//        m_p_Plane->putstr(m_Line+,0,"Exit! code:" + exit_code);
//        exit(exit_code);

    } else if(strcmp(args[0] ,"cd") == 0){
        auto  it = Tab::command_map.find("cd");
        if (it == command_map.end()) {
            m_p_Plane->printf(++Tab::m_Line, NCALIGN_CENTER, "cd command not found in command_map");
        } else {
            auto output = it->second->execute(args);
            if (!output.status_message.empty()) {
                m_p_Plane->printf(++Tab::m_Line, NCALIGN_CENTER, "%s", output.status_message.c_str());
            }
        }
        Tab::update_current_path();
    } else if(strcmp(args[0] ,"history") == 0){
        for(const auto &it: Tab::m_CommandHistory){
            m_p_Plane->putstr(++Tab::m_Line, NCALIGN_CENTER, it.c_str());
        }
    }else{
        if (!m_Command.empty() && m_Command != "\n") {
            if (Tab::handle_command(args) == EXIT_FAILURE) {
                Tab::m_p_Plane->printf(++Tab::m_Line, NCALIGN_CENTER, "Failed to executing command : `%s` ",
                                       m_Command.c_str());
            }
        }
    }

    Tab::m_Line +=1;
    Tab::m_Command.clear();
    Tab::free_args(args);
    Tab::m_CursorIdx = 0;
    Tab::update_current_path();
    Tab::m_CommandIdx += 1;
}

int Tab::handle_default(uint32_t m_Key) {
    if (m_Key >= 32 && m_Key <= 126) {
        m_Command.insert(m_Command.begin() + m_CursorIdx, static_cast<char>(m_Key));
        m_CursorIdx++;
    } else if (m_Key >= 0x80 && m_Key <= 0x10FFFF) {
        unsigned char utf8_buf[5] = {0};
        if (ncpp::NotCurses::ucs32_to_utf8(&m_Key, 1, utf8_buf, sizeof(utf8_buf))) {
            m_Command.insert(m_CursorIdx, reinterpret_cast<char *>(utf8_buf));
            m_CursorIdx++;
        }
    }

    return (int) m_ShellLen + m_CursorIdx; // new cursor position
}

void Tab::handle_backspace_press(int dim_x) {
    if (m_Command.empty()) return;
    Tab::m_Command.pop_back();
    // start from row, start col,num of row to erase,erase col location
    ncplane_erase_region(*m_p_Plane, m_Line, m_ShellLen, 1, dim_x - m_ShellLen); // causes flickering

}