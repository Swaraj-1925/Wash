//
// Created by swaraj on 6/20/25.
//

#include "cd.h"

Output CdCommand::execute(const std::vector<char *>& args) {
        parse_args(args);
        if(invalid){
            return Output {
                FAILURE_INVALID_INPUT,
                "cd, invalid flag"
            };
        }
        if (use_physical) {
            char resolved[PATH_MAX];
            if (realpath(path.c_str(), resolved) != nullptr) {
                path = std::string(resolved);
            } else {
                return Output {
                        FAILURE_STRING_OUTPUT,
                        "cd, cannot resolve path: " + path };
            }
        }

        if (check_exists && access(path.c_str(), F_OK) != 0) {
            return Output{FAILURE_STRING_OUTPUT, "cd, path does not exist: " + path};
        }
        if (chdir(path.c_str()) != 0) {
            return Output{FAILURE_INTERNAL_ERROR,"Failed to change directory to '" + path + "': " + std::strerror(errno)};
        }
        return {};
    }
    void CdCommand::parse_args(const std::vector<char *>& args) {
        for (size_t i = 1; i < args.size() && args[i]; ++i) {
            const char* arg = args[i];

            if (strcmp(arg, "-P") == 0) {
                use_physical = true;
            } else if (strcmp(arg, "-L") == 0) {
                use_physical = false;
            } else if (strcmp(arg, "-e") == 0) {
                check_exists = true;
            } else if (arg[0] != '-') {
                path = arg;
            } else {
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