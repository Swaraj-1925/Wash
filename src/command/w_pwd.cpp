//
// Created by swaraj on 6/23/25.
//

#include "w_pwd.h"

Output PwdCommand::execute(const std::vector<std::string> &args) {
    PwdCommand::text.clear();

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    std::string path(cwd);
    PwdCommand::text = path;
    return {.status_code = SUCCESS,.text_output = text};
}

int PwdCommand::render_output(ncpp::Plane *plane, Output output, int line) {
    std::string result = output.text_output;
    plane->printf(line,NCALIGN_LEFT,"%s",result.c_str());
    return 1;
}
