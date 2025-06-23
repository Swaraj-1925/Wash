//
// Created by swaraj on 6/23/25.
//

#include "whoami.h"

Output WhoamiCommand::execute(const std::vector<std::string> &args) {
    WhoamiCommand::text.clear();

    passwd* pw = getpwuid(getuid());
    WhoamiCommand::text = pw ? pw->pw_name:"unknown";
    return {.status_code = SUCCESS,.text_output = text};
}

int WhoamiCommand::render_output(ncpp::Plane *plane, Output output, int line) {
    std::string result = output.text_output;
    plane->printf(line,NCALIGN_LEFT,"%s",result.c_str());
    return 1;
}
