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
    Tab::m_p_Plane->set_scrolling(true);
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
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    if(errors.empty()){
        errors.emplace_back("==Congratulation!! NO ERRORS ==");
        std::copy(Tab::errors.begin(), Tab::errors.end(),output_iterator);
    } else{
        std::copy(Tab::errors.begin(), Tab::errors.end(),output_iterator);
    }
}
//int Tab::parse_and_execute_command(const std::string &line) {
//    std::istringstream iss(line);
//    std::string cmd;
//    iss >> cmd;
//
//    std::vector<std::string> args;
//    std::string arg;
//    while (iss >> arg) args.push_back(arg);
//    auto it = command_map.find(cmd);
//
//    if ( it == command_map.end()) {
////        Tab::m_p_Plane->printf(20,NCALIGN_CENTER,"Didnt find command: %s",cmd.c_str());
//        return EXIT_FAILURE;
//    }
//
//    Tab::m_output = it->second->execute(args);
//
//    if (Tab::m_output.status_code == FAILURE_VECTOR_OUTPUT){
//        for (auto  it:m_output.error_details) {
//            Tab::m_p_Plane->printf(Tab::m_Line++,NCALIGN_CENTER," %s",it.c_str());
//        }
//    } else if (Tab::m_output.status_code == FAILURE_STRING_OUTPUT){
//        Tab::m_p_Plane->printf(Tab::m_Line++,NCALIGN_CENTER," %s",m_output.status_message.c_str());
//    } else if (Tab::m_output.status_code == FAILURE_INVALID_INPUT){
//        Tab::m_p_Plane->printf(Tab::m_Line++,NCALIGN_CENTER," %s",m_output.status_message.c_str());
//    }
//    else if (COMMAND_WITH_OUTPUT.contains(cmd)){
//        int update_line = it->second->render_output(m_p_Plane,m_output,m_Line);
//        m_Line += update_line;
//    }
//
//    m_p_Plane->set_fg_default();
//    return EXIT_SUCCESS;
//}
void Tab::handle_prompt() {
    Tab::m_p_Plane->printf(Tab::m_Line,0, "%s %s \n",Tab::m_SHELL.c_str(),Tab::m_Command.c_str());
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
