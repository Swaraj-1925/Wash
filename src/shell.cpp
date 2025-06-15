//
// Created by swaraj on 6/4/25.
//
#include <shell.h>


// TODO: Mapping Operation we can perform on plane with the current mode
// TODO: Make Tab scrollable

Shell::Shell(ncpp::Plane *p_stdPlane, ncpp::NotCurses &nc,unsigned DimY,unsigned DimX)
        : m_p_StdPlane(p_stdPlane),
          m_DimY(DimY),
          m_DimX(DimX),
          m_Nc(nc)
{

    m_Start = std::chrono::high_resolution_clock::now();

    //create initial tab
    Shell::m_Tab = Tab(m_p_StdPlane,m_DimY,m_DimX,0);
    m_Tabs.push_back(m_Tab);

    //initialize status line
    i_p_StatusLine = StatusLine(m_p_StdPlane,m_DimY,m_DimX);
    i_p_StatusLine.render_status_line(m_Tabs);
//    Shell::create_tab();
    // setup courser and enable mouse
    m_Nc.cursor_enable(m_Tab.m_Line,m_Tab.m_ShellLen);
    m_Nc.mouse_enable(NCMICE_ALL_EVENTS);
}
Shell::~Shell() {
    m_Nc.stop();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - m_Start );
    std::cout << "Wash shell exited successfully! , it was active for " << duration << " seconds \n";

}
int Shell::ctrl_c_press_count = 0;


void Shell::handle_ctrl_c(int sig) {++ctrl_c_press_count;}

int Shell::create_tab() {
    for (auto &tab : m_Tabs) {tab.m_Active = false;}
    int tab_idx = (int)Shell::m_Tabs.size();
    Tab newTab = Tab(m_p_StdPlane,m_DimY,m_DimX,tab_idx);
    m_Tabs.push_back(newTab);
    i_p_StatusLine.render_status_line(m_Tabs);
    Shell::m_Tab = newTab;
    return EXIT_SUCCESS;
}


