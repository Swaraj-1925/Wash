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

// permission
//4 = read (r)
//2 = write (w)
//1 = execute (x)
//0 = no permissions
//Combinations: 6 = 4+2 (read + write), 4 = read only, etc.
// owner,group,other
//The number is split into three digits (plus a leading 0 for octal)

#define TOUCH_FILE_PERMISSION 0644
#define TOUCH_FLAG (O_CREAT | O_WRONLY)


// output
enum StatusCode {
    SUCCESS = 200,
    SUCCESS_VECTOR_OUTPUT = 201,
    SUCCESS_STRING_OUTPUT = 202,

    FAILURE = 400,
    FAILURE_INVALID_INPUT = 401,
    FAILURE_VECTOR_OUTPUT = 410,
    FAILURE_STRING_OUTPUT = 411,
    FAILURE_INTERNAL_ERROR = 500,

    FEATURE_NOT_IMPLEMENTED = 501
};

#endif //WASH_CONSTANTS_H