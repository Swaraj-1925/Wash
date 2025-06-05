#include <cstdlib>
#include <clocale>
#include <iostream>
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <chrono>


#include "src/utils/options.h"
#include "src/shell.cpp"
int main() {
    if (setlocale(LC_ALL, "") == nullptr) {
        return EXIT_FAILURE;
    }
    struct notcurses_options nc_opts = default_notcurses_options();

    ncpp::NotCurses nc(nc_opts);
    if (!nc) return EXIT_FAILURE;


    ncpp::Plane* p_std_plane = nc.get_stdplane();
    if (!p_std_plane) {
        nc.stop();
        std::cerr << "Failed to get std_plane \n";
        return EXIT_FAILURE;
    }

    Shell sh(p_std_plane,nc);
    int shell_result = sh.runShell();

    return shell_result;
}
