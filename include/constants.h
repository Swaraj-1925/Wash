//
// Created by swaraj on 5/16/25.
//

#ifndef WASH_CONSTANTS_H
#define WASH_CONSTANTS_H
#include <unordered_set>

// notcurses_option margin values
#define TERM_TYPE NULL
// Margins
#define MARGIN_T 0
#define MARGIN_R 0
#define MARGIN_B 0
#define MARGIN_L 0

// Logging Levels
#define LOG_LEVEL_DEV NCLOGLEVEL_INFO     // Development logging
#define LOG_LEVEL_PROD NCLOGLEVEL_SILENT  // Production logging

// Notcurses Flags
#define FLAGS (NCOPTION_SUPPRESS_BANNERS | NCOPTION_INHIBIT_SETLOCALE | NCOPTION_PRESERVE_CURSOR)

// main plain Flags
#define MAIN_PLAIN_FLAGS (NCPLANE_OPTION_VSCROLL | NCPLANE_OPTION_AUTOGROW)

#endif //WASH_CONSTANTS_H
