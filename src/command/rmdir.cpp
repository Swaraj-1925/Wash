//
// Created by swaraj on 6/23/25.
//

#include "rmdir.h"

bool RmdirCommand::remove_dir(const std::filesystem::path dir, Output &res) {
    std::error_code ec;
    bool success = true;

    if (std::filesystem::exists(dir)){
        res.status_code = FAILURE_VECTOR_OUTPUT;
        res.error_details.push_back("Cannot remove '" + dir.string() + "': No such file or directory\n");
        return false;
    }
    if (std::filesystem::is_directory(dir,ec)){
        res.status_code = FAILURE_VECTOR_OUTPUT;
        res.error_details.push_back("Cannot remove '" + dir.string() + "': Not a directory\n");
        return false;
    }
    std::filesystem::remove(dir, ec);
    if (ec) {
        if (ec.value() == ENOTEMPTY && ignore_fail_on_non_empty) {
            return true;
        }
        res.error_details.push_back("Cannot remove '" + dir.string() + "': " + ec.message() );
        return false;
    }
    if (verbose){
        res.lines_output.push_back("\"Removed directory '\" + dir.string()");
    }
    // Handle parent directories if -p is specified
    if (parents){
        res.status_code = FEATURE_NOT_IMPLEMENTED;
        return false;
    }
    return true;
}
Output RmdirCommand::execute(const std::vector <std::string> &args) {
    parse_args(args);
    Output res;
    for (const auto& dir : directories) {
        RmdirCommand::remove_dir(dir, res);
    }
    return res;
}

void RmdirCommand::parse_args(const std::vector <std::string> &args) {
    verbose = false;
    parents = false;
    ignore_fail_on_non_empty = false;
    directories.clear();
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-v" || args[i] == "--verbose") {
            verbose = true;
        } else if (args[i] == "-p" || args[i] == "--parents") {
            parents = true;
        } else if (args[i] == "--ignore-fail-on-non-empty") {
            ignore_fail_on_non_empty = true;
        } else if (args[i][0] == '-') {
            continue;
        } else {
            directories.push_back(args[i]);
        }
    }

}