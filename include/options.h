//
// Created by swaraj on 5/16/25.
//

#ifndef WASH_OPTIONS_H
#define WASH_OPTIONS_H
#include "constants.h"
#include <ncpp/NotCurses.hh>
#include <ncpp/Reel.hh>
// Default notcurses options
static struct notcurses_options default_notcurses_options() {

    notcurses_options nc_opts{};

    nc_opts.termtype = TERM_TYPE;
    nc_opts.loglevel = LOG_LEVEL_PROD;
    nc_opts.margin_t = MARGIN_T;
    nc_opts.margin_r = MARGIN_R;
    nc_opts.margin_b = MARGIN_B;
    nc_opts.margin_l = MARGIN_L;
    nc_opts.flags = FLAGS;

    return nc_opts;
}



static struct ncplane_options default_tab_option(unsigned DimX, unsigned DimY) {
    ncplane_options nc_opts{};
    nc_opts.y = 0;
    nc_opts.x = 0;
    nc_opts.rows = DimY-2;
    nc_opts.cols = DimX;
    nc_opts.name = "tab_plain";
    nc_opts.flags = MAIN_PLAIN_FLAGS;

    return nc_opts;
}
static struct ncplane_options default_statusLine_option(unsigned DimY, unsigned DimX) {
    ncplane_options nc_opts{};
    nc_opts.y = DimY-1;
    nc_opts.x = 0;
    nc_opts.rows = 2;
    nc_opts.cols = DimX;

    nc_opts.userptr = nullptr;
    nc_opts.flags = NCPLANE_OPTION_FIXED;
    nc_opts.name = "status_line";
    return nc_opts;
}

static struct ncplane_options default_mode_option(unsigned DimY, unsigned DimX) {

    ncplane_options nc_opts{};
    nc_opts.y = DimY-1;
    nc_opts.x = 0;
    nc_opts.rows = 2;
    nc_opts.cols = 10;

    nc_opts.userptr = nullptr;
    nc_opts.flags = NCPLANE_OPTION_FIXED;
    nc_opts.name = "prompt";
    return nc_opts;
}
static struct ncplane_options default_statusLine_tab_option() {
    ncplane_options nc_opts{};

    return nc_opts;
}
#endif //WASH_OPTIONS_H
