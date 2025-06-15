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
    if(!nc){
        std::cout<<"not abel to get not curser";
        return EXIT_FAILURE;

    }
    unsigned DimY,DimX;
    nc.get_term_dim(&DimY, &DimX);
    if (!nc) return EXIT_FAILURE;


    ncpp::Plane* p_std_plane = nc.get_stdplane();
    Theme theme;

    ncpp::Cell base(' ');
    base.set_bg_rgb8(theme.TERM_CRUST_BG.get_r(),
                     theme.TERM_CRUST_BG.get_g(),
                     theme.TERM_CRUST_BG.get_b());
    p_std_plane->set_base_cell(base);
    p_std_plane->erase();

    if (!p_std_plane) {
        nc.stop();
        std::cerr << "Failed to get std_plane \n";
        return EXIT_FAILURE;
    }

    Shell sh(p_std_plane,nc,DimY,DimX);
    int shell_result = sh.run_shell();
    return shell_result;
}