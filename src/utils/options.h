//
// Created by swaraj on 5/16/25.
//

#ifndef WASH_OPTIONS_H
#define WASH_OPTIONS_H
#include "constants.h"
#include <notcurses/notcurses.h>
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

static struct ncplane_options main_plane_option(unsigned dim_x, unsigned dim_y) {
    ncplane_options nc_opts{};
    nc_opts.y = 0;
    nc_opts.x = 0;
    nc_opts.rows = 3;
    nc_opts.cols = dim_x - 4;
    nc_opts.userptr = nullptr;
    nc_opts.name = "reader";
    nc_opts.resizecb = nullptr;
    nc_opts.flags = 0;
    nc_opts.margin_b = 0;
    nc_opts.margin_r = 0;
    return nc_opts;
}

#endif //WASH_OPTIONS_H
