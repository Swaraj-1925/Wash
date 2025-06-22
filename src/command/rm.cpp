//
// Created by swaraj on 6/23/25.
//

#include "rm.h"

Output RmCommand::execute(const std::vector <std::string> &args) {
    parse_args(args);
    Output result;
    if(files.empty()) {
        result.status_code = FAILURE_STRING_OUTPUT;
        result.text_output = "No files specified";
        return result;
    }
    for (const auto& file : files) {
        std::error_code ec;
        bool removed = false;

        if (recursive && std::filesystem::is_directory(file, ec)) {
            // Remove directory and contents recursively
            removed = std::filesystem::remove_all(file, ec) > 0;
        } else {
            // Remove single file
            removed = std::filesystem::remove(file, ec);
        }
        if (ec) {
            if (!force || (force && ec.value() != ENOENT)) {
                result.lines_output.push_back( "Failed to remove '" + file + "': " + ec.message());
                result.status_code = FAILURE_VECTOR_OUTPUT;
            }
        } else if (!removed && !force) {
            // Nothing was removed (e.g., file doesn't exist) and -f not specified
            result.lines_output.push_back("Failed to remove '" + file + "': No such file or directory\n");
            result.status_code = FAILURE_VECTOR_OUTPUT;
        }
    }
    return result;
}
void RmCommand::parse_args(const std::vector <std::string> &args) {
    recursive = false;
    force = false;
    files.clear();
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-r" || args[i] == "-R") {
            recursive = true;
        } else if (args[i] == "-f") {
            force = true;
        } else if (args[i][0] == '-') {
            continue;
        } else {
            files.push_back(args[i]);
        }
    }
}