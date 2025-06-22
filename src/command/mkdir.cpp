//
// Created by swaraj on 6/22/25.
//
#include "mkdir.h"

Output MkdirCommand::execute(const std::vector<std::string> &args) {
    parse_args(args);
    Output result;
    result.status_code = 200;
    for (const auto &dir : directories) {
        std::error_code ec;
        if (parents) {
            std::filesystem::create_directories(dir, ec);
        } else {
            std::filesystem::create_directory(dir, ec);
        }
        if (ec) {
            result.status_code = 500;
            result.message += "mkdir: cannot create directory '" + dir + "': " + ec.message() + "\n";
        } else {
            if (set_mode) {
                chmod(dir.c_str(), mode);
            }
            if (verbose) {
                result.string_output.push_back("mkdir: created directory '" + dir );
            }
        }
    }
    return result;
}

void MkdirCommand::parse_args(const std::vector<std::string> &args) {
    parents = false;
    verbose = false;
    set_mode = false;

    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "-p") MkdirCommand::parents = true;
        else if (args[i] == "-v") MkdirCommand::verbose = true;
        else if (args[i] == "-m") {
            std::string mode_str;
            if (i + 1 >= args.size()) {
                MkdirCommand::err = "mkdir: option requires an argument -- 'm'";
            }
            mode_str = args[++i];
            mode = static_cast<mode_t>(std::stoul(mode_str, nullptr, 8));  // octal!
            set_mode = true;
        } else if (args[i].starts_with("--mode=")){
            std::string mode_str;
            size_t pos = args[i].find('=');
            if (pos == std::string::npos || pos + 1 >= args[i].size()) {
                MkdirCommand::err = "mkdir: invalid mode syntax";
            }
            mode_str = args[i].substr(pos + 1);
            try {
                mode = static_cast<mode_t>(std::stoul(mode_str, nullptr, 8));  // octal!
                set_mode = true;
            } catch (...) {
                MkdirCommand::err = "mkdir: invalid mode '" + mode_str + "'";
            }
        }
        else MkdirCommand::directories.push_back(args[i]);
    }
}
int MkdirCommand::render_output(ncpp::Plane *plane, Output output, int line) {
    std::vector<std::string> result = output.string_output;
    if (MkdirCommand::verbose){
        for (const auto &it: result) {
            plane->printf(line,NCALIGN_LEFT, "%s", it.c_str());
        }
        return result.size();
    }
    return 0;
}