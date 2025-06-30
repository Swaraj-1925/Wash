//
// Created by swaraj on 6/15/25.
//

#include "tab.h"


Tab::Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, std::string name)
:DimY(dim_y),DimX(dim_x)
{
    Tab::m_p_StdPlane = std_plane;

//     initialize plane
    struct ncplane_options TabOpts = default_tab_option(DimY, DimX);
    Tab::m_p_Plane = new ncpp::Plane( *Tab::m_p_StdPlane, &TabOpts);


//    m_p_NotificationPlane->move_bottom();
    auto res = ncplane_set_scrolling(*m_p_Plane, true);
//    Tab::debug.push_back(std::string("set scrolling output: ") + (res == 0 ? "success" : "failure"));
    Tab::m_p_Plane->erase();
    ncpp::Cell base(' ');

    base.set_bg_rgb8(t.TERM_BASE_BG.r(),
                     t.TERM_BASE_BG.g(),
                     t.TERM_BASE_BG.b());

    base.set_fg_rgb8(t.TEXT0.r(),
                     t.TEXT0.g(),
                     t.TEXT0.b());
    Tab::m_p_Plane->set_base_cell(base);
    Tab::m_p_Plane->erase();

    Tab::m_Active = true;
    Tab::m_Name = name;

    // meta data
    passwd* pw = getpwuid(getuid());
    Tab::m_HomeDir = pw ? pw->pw_dir:"";
    Tab::m_Username = pw ? pw->pw_name:"unknown";

    Tab::m_CurrentPath = Tab::m_HomeDir;

    // initialize prompt
    Tab::update_current_path();
    Tab::register_builtin_commands();
}
void Tab::update_current_path() {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    std::string path(cwd);

    const char* home = getenv("HOME");

    if (home && path.find(home) == 0) {
        path.replace(0, strlen(home), "~");
    }

    Tab::m_CurrentPath = path;

    Tab::m_SHELL = "[" + m_Username + "@ " + m_CurrentPath + "]";
    Tab::m_ShellLen = (int)Tab::m_SHELL.length() + 1;
}

Tab::~Tab(){
    std::ofstream output_file("error.txt");
    std::ofstream debug_file("debug.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    if(errors.empty()){
        errors.emplace_back("==Congratulation!! NO ERRORS ==");
        std::copy(Tab::errors.begin(), Tab::errors.end(),output_iterator);
    } else{
        std::copy(Tab::errors.begin(), Tab::errors.end(),output_iterator);
    }
    std::ostream_iterator<std::string> debug_iterator(debug_file, "");
    std::copy(debug.begin(), debug.end(), debug_iterator);
}
void Tab::handle_prompt() {
    m_p_Plane->cursor_move(m_Line, 0);

    m_p_Plane->printf(Tab::m_Line,0,"%s",m_SHELL.c_str());
    m_ShellLen = m_SHELL.size() + 1;
    m_p_Plane->printf(Tab::m_Line,Tab::m_ShellLen,"%s",m_Command.c_str());

    if (m_CursorIdx < 0) m_CursorIdx = 0;
    if (m_CursorIdx > (int)m_Command.size()) m_CursorIdx = m_Command.size();

    // Move cursor (strictly after shell)
    int cursor_x = m_ShellLen + m_CursorIdx;
    m_p_Plane->cursor_move(m_Line, cursor_x);

    // start from row, start col,num of row to erase,erase col location
}
void Tab::free_args(std::vector<char*>& args) {
    for (auto* ptr : args) {
        delete[] ptr;
    }
    args.clear();
}

std::string Tab::get_mode() {
    switch (m_Mode) {
        case 0:
            return "INSERT";
            break;
        case 1:
            return "COMMAND";
            break;
        case 2:
            return "VISUAL";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}
