//
// Created by swaraj on 6/23/25.
//

#include "mv.h"

void MvCommand::parse_args(const std::vector <std::string> &args) {
    force = false;
    verbose = false;
    sources.clear();
    target.clear();

    // Parse arguments
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-f" || args[i] == "--force") {
            force = true;
        } else if (args[i] == "-v" || args[i] == "--verbose") {
            verbose = true;
        } else if (args[i][0] == '-') {
            continue;
        } else if (i == args.size() - 1) {
            target = args[i];
        } else {
            sources.push_back(args[i]);
        }
    }
}
bool MvCommand::move(const std::filesystem::path &src, const std::filesystem::path &dst, Output &res) {

    std::error_code ec;
    if (!std::filesystem::exists(src, ec)) {
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message =  "Cannot move '" + src.string() + "': No such file or directory\n";
        return false;
    }
    if(!force && std::filesystem::exists(dst)){
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message = "Cannot move '" + dst.string() + "': Destination already exists\n";
        return false;
    }
    std::filesystem::rename(src,dst,ec);
    if (ec) {
        res.status_code = FAILURE_INTERNAL_ERROR;
        res.status_message = "Failed to move '" + src.string() + "' to '" + dst.string() + "': " + ec.message() + "\n";
        return false;
    }
    if (verbose){
        res.text_output = "Copied '" + src.string() + "' to '" + dst.string() + "'\n";
    }
    return true;
}

Output MvCommand::execute(const std::vector <std::string> &args) {
    parse_args(args);
    Output res;
    std::filesystem::path target_path(target);
    bool target_is_dir = std::filesystem::is_directory(target_path);

    if (sources.size() > 1 && !target_is_dir) {
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message = "Cannot copy multiple sources to a non-directory target\n";
        return res;
    }
    for (const auto& src : sources) {
        std::filesystem::path src_path(src);
        std::filesystem::path dst_path = target_is_dir ? target_path /src_path.filename() : target_path;

        MvCommand::move(src_path,dst_path,res);
    }
    return res;
}