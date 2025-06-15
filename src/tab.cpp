//
// Created by swaraj on 6/15/25.
//

#include "tab.h"

Tab::Tab(ncpp::Plane* std_plane,unsigned dim_y, unsigned dim_x, int tab_index)
{
    Tab::m_p_StdPlane = std_plane;

//     initialize plane
    struct ncplane_options TabOpts = default_tab_option(dim_y, dim_x);
    Tab::m_p_Plane = new ncpp::Plane( *Tab::m_p_StdPlane, &TabOpts);
    Tab::m_p_Plane->set_scrolling(true);
    Tab::m_p_Plane->erase();
    ncpp::Cell base(' ');

    base.set_bg_rgb8(t.TERM_BASE_BG.get_r(),
                     t.TERM_BASE_BG.get_g(),
                     t.TERM_BASE_BG.get_b());

    base.set_fg_rgb8(t.TEXT0.get_r(),
                     t.TEXT0.get_g(),
                     t.TEXT0.get_b());
    Tab::m_p_Plane->set_base_cell(base);
    Tab::m_p_Plane->erase();

    Tab::m_Active = true;
    Tab::m_Name = "Tab " + std::to_string(tab_index);

    // meta data
    struct passwd* pw = getpwuid(getuid());
    Tab::m_HomeDir = pw ? pw->pw_dir:"";
    Tab::m_Username = pw ? pw->pw_name:"unknown";
    Tab::m_CurrentPath = Tab::m_HomeDir;

    // initialize prompt
    Tab::m_SHELL = "[" + m_Username + "@ " + m_CurrentPath + "]";
    Tab::m_ShellLen = (int)Tab::m_SHELL.length() + 1;
}
void Tab::update_current_path() {
    if(m_CurrentPath == m_HomeDir) m_CurrentPath = "~";

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    std::string path(cwd);

    const char* home = getenv("HOME");
    if (home && path.find(home) == 0) {
        path.replace(0, strlen(home), "~");
    }
    Tab::m_CurrentPath = path;
}

void Tab::handle_prompt() {
    Tab::m_p_Plane->printf(Tab::m_Line,ncpp::NCAlign::Left, "%s", Tab::m_SHELL.c_str());
    Tab::m_p_Plane->printf(Tab::m_Line,Tab::m_ShellLen, "%s", Tab::m_Command.c_str());
}