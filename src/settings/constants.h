//
// Created by swaraj on 5/16/25.
//

#ifndef WASH_CONSTANTS_H
#define WASH_CONSTANTS_H

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
#define FLAGS (NCOPTION_SUPPRESS_BANNERS | NCOPTION_INHIBIT_SETLOCALE)

#endif //WASH_CONSTANTS_H
