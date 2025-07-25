//
// Created by swaraj on 6/13/25.
//

#ifndef wash_shortcut_h
#define wash_shortcut_h

// Key definitions for Wash shell commands
#define WS_KEY_QUIT 'D'              // Ctrl+D to quit the shell
#define WS_KEY_TOGGLE_STATUS_LINE 'S'     // Ctrl+S to toggle status line
#define WS_KEY_NEW_TAB 'a'           // 'A' to create a new tab
#define WS_KEY_TOGGLE_STATUS_LINE_MCOMMAND ':'     // Ctrl+S to toggle status line
#define WS_KEY_TOGGLE_STATUS_LINE_MVISUAL 'v'     // Ctrl+S to toggle status line
#define WS_KEY_DELETE_TAB 'x'
//#define WS_KEY_SEND_BACKGROUND 'B'
// Shortcut conditions for input handling
#define WS_ENTER (m_Key == NCKEY_ENTER)
#define WS_BACKSPACE (m_Key == NCKEY_BACKSPACE )
#define WS_QUIT (m_Key == WS_KEY_QUIT && ncinput_ctrl_p(&m_NcIn))
#define WS_HISTORY_UP (m_Key == NCKEY_UP)
#define WS_HISTORY_DOWN (m_Key == NCKEY_DOWN)

#define WS_NEW_TAB (m_Key == WS_KEY_NEW_TAB)
#define WS_DELETE_TAB (m_Key == WS_KEY_DELETE_TAB)
#define WS_MOVE_LEFT_TAB (m_Key == NCKEY_LEFT)
#define WS_MOVE_RIGHT_TAB (m_Key == NCKEY_RIGHT)
#define WS_TOGGLE_STATUS_LINE (m_Key == WS_KEY_TOGGLE_STATUS_LINE && ncinput_ctrl_p(&m_NcIn))
#define WS_STATUS_LINE_MCOMMAND (m_Key == WS_KEY_TOGGLE_STATUS_LINE_MCOMMAND)
#define WS_STATUS_LINE_MVISUAL (m_Key == WS_KEY_TOGGLE_STATUS_LINE_MVISUAL)

#define WS_TAB (m_Key == NCKEY_TAB)


#define WS_ARROW_LEFT (m_Key == NCKEY_LEFT)
#define WS_ARROW_RIGHT (m_Key == NCKEY_RIGHT)

#define WS_CTRL_A (m_Key == 'A' && ncinput_ctrl_p(&m_NcIn))
#define WS_CTRL_B (m_Key == 'B' && ncinput_ctrl_p(&m_NcIn))
#define WS_CTRL_E (m_Key == 'E' && ncinput_ctrl_p(&m_NcIn))
#define WS_CTRL_F (m_Key == 'F' && ncinput_ctrl_p(&m_NcIn))

//#define WS_SEND_BACKGROUND (m_Key == WS_KEY_SEND_BACKGROUND && ncinput_ctrl_p(&m_NcIn))
#endif //wash_shortcut_h
