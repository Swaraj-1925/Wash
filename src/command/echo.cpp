//
// Created by swaraj on 6/22/25.
//
#include "echo.h"

Output EchoCommand::execute(const std::vector<std::string> &args) {
    EchoCommand::text.clear();
    for (const auto& arg : args) {
        if (arg.starts_with('$')) {
            const char* env = getenv(arg.substr(1).c_str());
            EchoCommand::text += env == nullptr ? "" : env;
            EchoCommand::text += "\t";
        }
        else EchoCommand::text += arg + "\t";
    }

    return {.status_code = SUCCESS,.text_output = text};
}

int EchoCommand::render_output(ncpp::Plane *plane, Output output, int line) {
    std::string result = output.text_output;
    plane->printf(line+1,NCALIGN_LEFT,"%s",EchoCommand::text.c_str());
    return 1;
}