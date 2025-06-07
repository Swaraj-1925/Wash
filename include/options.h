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
//    nc_opts.flags = F;

    return nc_opts;
}



static struct ncplane_options default_main_plain_option(unsigned DimX, unsigned DimY) {
    ncplane_options nc_opts{};
    nc_opts.y = 0;
    nc_opts.x = 0;
    nc_opts.rows = DimY;
    nc_opts.cols = DimX-2;
    nc_opts.name = "main_plain";
//    nc_opts.flags = MAIN_PLAIN_FLAGS;

    return nc_opts;
}
static struct ncplane_options default_status_line_option(unsigned DimX, unsigned DimY) {
    ncplane_options nc_opts{};

    nc_opts.y = DimY - 1;
    nc_opts.x = 0;
    nc_opts.rows = 1;
    nc_opts.cols = DimY;
    nc_opts.name = "status_line";

    return nc_opts;
}

static struct ncreel_options default_status_line_reel_option() {
    ncreel_options nc_opts{};
    ncchannels_set_fg_rgb(&nc_opts.focusedchan, 0xffffff);
    ncchannels_set_bg_rgb(&nc_opts.focusedchan, 0x00c080);
    ncchannels_set_fg_rgb(&nc_opts.borderchan, 0x00c080);

    return nc_opts;
}
#endif //WASH_OPTIONS_H
