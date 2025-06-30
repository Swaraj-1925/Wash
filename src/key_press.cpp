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
    for (const auto* arg : args) {
        debug.push_back(std::string("debug args cd : ") + (arg ? arg : "nullptr") + "\n");
    }
    if(strcmp(args[0] ,"clear") == 0){
        Tab::m_p_Plane->erase();
        Tab::m_Line = 0;
    } else if(strcmp(args[0] ,"exit") == 0){
        int exit_code = 0;
        if(args[1] != nullptr){
            exit_code = strtol(args[1], nullptr,10);
        }
        Tab::m_Line +=1;
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
        Tab::m_Line +=1;
        Tab::update_current_path();
    } else if(strcmp(args[0] ,"history") == 0){
        for(const auto &it: Tab::m_CommandHistory){
            m_p_Plane->putstr(++Tab::m_Line, NCALIGN_CENTER, it.c_str());
        }
        Tab::m_Line +=1;
    }else if(strcmp(args[0] ,"cat") == 0){
        auto it = Tab::command_map.find("cat");
        if (it == command_map.end()) {
            m_p_Plane->printf(++Tab::m_Line, NCALIGN_CENTER, "cat command not found in command_map");
        } else {
            auto output = it->second->execute(args);
            if (output.status_code == SUCCESS || !output.lines_output.empty()) {
                for (const auto& line : output.lines_output) {
                    m_p_Plane->putstr(++Tab::m_Line, 0, line.c_str());
                }
            }
            if (!output.status_message.empty()) {
                m_p_Plane->printf(++Tab::m_Line, 0, "%s", output.status_message.c_str());
            }

            for (const auto& error : output.error_details) {
                m_p_Plane->printf(++Tab::m_Line, 0, "%s", error.c_str());
            }
        }
    }else{
        if (!m_Command.empty() && m_Command != "\n") {
            if (Tab::handle_command(args) == EXIT_FAILURE) {
                Tab::m_p_Plane->printf(++Tab::m_Line, NCALIGN_CENTER, "Failed to executing command : `%s` ",
                                       m_Command.c_str());
            }
        }
        Tab::m_Line +=1;
    }

    Tab::m_Command.clear();
    Tab::free_args(args);
    Tab::m_CursorIdx = 0;
    Tab::update_current_path();
    Tab::m_CommandIdx += 1;
    ncplane_erase_region(*m_p_Plane, m_Line, 0, INT_MAX, INT_MAX); // causes flickering
}

int Tab::handle_default(uint32_t m_Key) {
    if (m_Key >= 32 && m_Key <= 126) {
//        m_Command.insert(m_Command.begin() + m_CursorIdx, static_cast<char>(m_Key));

        m_Command.insert(m_CursorIdx, 1, static_cast<char>(m_Key));
        m_CursorIdx++;
    } else if (m_Key >= 0x80 && m_Key <= 0x10FFFF) {
        unsigned char utf8_buf[5] = {0};
        if (ncpp::NotCurses::ucs32_to_utf8(&m_Key, 1, utf8_buf, sizeof(utf8_buf))) {
//            m_Command.insert(m_CursorIdx, reinterpret_cast<char *>(utf8_buf));
//            m_CursorIdx++;
            m_Command.insert(m_CursorIdx, reinterpret_cast<char *>(utf8_buf));
            m_CursorIdx += strlen(reinterpret_cast<char *>(utf8_buf));
        }
    }

    return (int) m_ShellLen + m_CursorIdx; // new cursor position
}

void Tab::handle_backspace_press(int dim_x) {
    if (m_Command.empty()) return;
//    Tab::m_Command.pop_back();
    m_Command.erase(m_CursorIdx - 1, 1);
    // start from row, start col,num of row to erase,erase col location
    ncplane_erase_region(*m_p_Plane, m_Line, m_ShellLen, 1, dim_x - m_ShellLen); // causes flickering
}
void Tab::handle_tab() {
    TabCompletion tb_completion(debug);
    auto out = tb_completion.handleTabCompletion(m_Command);
    if (!out.empty()) {
        // Tokenize the current command
        std::vector<std::string> tokens;
        std::stringstream ss(m_Command);
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        if (!tokens.empty()) {
            if (out.size() == 1) {
                // Replace the last token with the completion
                tokens.back() = out[0];
                // Rebuild the command
                std::string newCommand;
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if (i > 0) newCommand += " ";
                    newCommand += tokens[i];
                }
                // Add a space for further input
                newCommand += " ";
                m_Command = newCommand;
               m_CursorIdx = (int)newCommand.size();
                m_p_Plane->cursor_move(m_Line, m_CursorIdx);
            } else if (out.size() < 20) {
                for (const auto& it : out) {
                    size_t max_length = 0;
                    for (const auto& it : out) {
                        max_length = std::max(max_length, it.size());
                    }
                    int column_width = max_length + 2;
                    unsigned dimx = m_p_Plane->get_dim_x();
                    int num_columns = std::max(1, (int)(dimx/ column_width));
                    int num_rows = (out.size() + num_columns - 1) / num_columns;

                    // Clear the line below the prompt for display
                    ncplane_erase_region(*m_p_Plane, m_Line + 1, 0, 1, dimx);

                    // Print completions in a grid
                    for (int row = 0; row < num_rows; ++row) {
                        for (int col = 0; col < num_columns; ++col) {
                            size_t idx = row + col * num_rows;
                            if (idx < out.size()) {
                                // Format with fixed width
                                std::string padded = out[idx] + std::string(column_width - out[idx].size(), ' ');
                                m_p_Plane->printf(m_Line + 1, col * column_width, "%s", padded.c_str());
                            }
                        }
                    }
                }
            }
        }
    }
}