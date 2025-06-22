//
// Created by swaraj on 6/22/25.
//

#include "touch.h"

Output TouchCommand::execute(const std::vector <std::string> &args) {
    if (args.empty()) return {.status_code =FAILURE_INVALID_INPUT,.status_message="No filenames provided"};
    Output result;

    for (const auto& it:args) {
        int fd = open(it.c_str(), TOUCH_FLAG, TOUCH_FILE_PERMISSION);
        if (fd == -1){
            result.lines_output.push_back("Failed to touch '" + it + "': " + strerror(errno));
            result.status_code = FAILURE_VECTOR_OUTPUT;
        }
        if (futimens(fd, nullptr) == -1) {
            result.lines_output.push_back("Failed to update timestamps for '" + it + "': " + strerror(errno));
            result.status_code = FAILURE_VECTOR_OUTPUT;
        }
    }
    return  result;
}