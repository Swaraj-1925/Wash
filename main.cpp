#include <cstdlib>
#include <clocale>
#include <iostream>
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <string>


#include "options.h"
#include "theme.h"
#include <shell.h>

int main() {
    if (setlocale(LC_ALL, "") == nullptr) {
        return EXIT_FAILURE;
    }
    struct notcurses_options nc_opts = default_notcurses_options();

    ncpp::NotCurses nc(nc_opts);
    unsigned DimY,DimX;
    nc.get_term_dim(&DimY, &DimX);
    if (!nc) return EXIT_FAILURE;



    ncpp::Plane* p_std_plane = nc.get_stdplane();
    Theme theme;
    if (!p_std_plane->set_bg_rgb8(theme.TERM_BG.get_r(),theme.TERM_BG.get_g(),theme.TERM_BG.get_g() )){
        nc.stop();
        std::cerr<<"not abel to set color";
        return EXIT_FAILURE;
    }
    if (!p_std_plane) {
        nc.stop();
        std::cerr << "Failed to get std_plane \n";
        return EXIT_FAILURE;
    }

    Shell sh(p_std_plane,nc,DimY,DimX);
    int shell_result = sh.run_shell();
    return shell_result;
}