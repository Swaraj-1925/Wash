//
// Created by swaraj on 6/20/25.
//

#include "command/cd.h"

Output CdCommand::execute(const std::vector<std::string>& args) {
        parse_args(args);
        if(invalid){
            return Output {
                400,
                "cd, invalid flag"
            };
        }
        if (use_physical) {
            char resolved[PATH_MAX];
            if (realpath(path.c_str(), resolved) != nullptr) {
                path = std::string(resolved);
            } else {
                return Output {
                        400,
                        "cd, cannot resolve path: " + path };
            }
        }

        if (check_exists && access(path.c_str(), F_OK) != 0) {
            return Output{400, "cd, path does not exist: " + path};
        }
        if (chdir(path.c_str()) != 0) {
            return Output{400,"cd, " + path +  "\n"};
        }
        return Output{200};
    }
    void CdCommand::parse_args(const std::vector<std::string>& args) {
        for (const auto& arg : args) {
            if (arg == "-P") {
                use_physical = true;
            } else if (arg == "-L") {
                use_physical = false;
            } else if (arg == "-e") {
                check_exists = true;
            } else if (!arg.empty() && arg[0] != '-') {
                path = arg;
            } else{
                invalid = true;
            }
        }
        if (path.empty()) {
            const char* home = getenv("HOME");
            if (!home) {
                std::cerr << "cd: HOME not set\n";
                return;
            }
            path = home;
        }
    }