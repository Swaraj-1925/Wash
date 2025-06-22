//
// Created by swaraj on 6/23/25.
//

#include "cp.h"
bool CpCommand::copy(const std::filesystem::path &src, const std::filesystem::path &dst,bool is_src_dir,Output &res) {
    std::error_code ec;
    if (!std::filesystem::exists(src, ec)) {
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message =  "Cannot copy '" + src.string() + "': No such file or directory\n";
        return false;
    }
    if(!force && std::filesystem::exists(dst)){
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message = "Cannot copy '" + dst.string() + "': Destination already exists\n";
        return false;
    }
    if (is_src_dir){
        std::filesystem::copy_file(src,dst,std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive,ec);
    } else{
        std::filesystem::copy_file(src,dst,std::filesystem::copy_options::overwrite_existing,ec);
    }

    if (ec) {
        res.status_code = FAILURE_INTERNAL_ERROR;
        res.status_message = "Failed to copy '" + src.string() + "' to '" + dst.string() + "': " + ec.message() + "\n";
        return false;
    }
    if (verbose){
        res.text_output = "Copied '" + src.string() + "' to '" + dst.string() + "'\n";
    }
    return true;
}
void CpCommand::parse_args(const std::vector <std::string> &args) {
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-r" || args[i] == "-R" || args[i] == "--recursive") {
            recursive = true;
        } else if (args[i] == "-f" || args[i] == "--force") {
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
Output CpCommand::execute(const std::vector <std::string> &args) {
    Output res;
    std::filesystem::path target_path(target);
    bool target_is_dir = std::filesystem::is_directory(target_path);

    if (sources.size() < 2) {
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message = "Too few arguments";
    }
    parse_args(args);
    if (sources.size() > 1 && !target_is_dir) {
        res.status_code = FAILURE_INVALID_INPUT;
        res.status_message = "Cannot copy multiple sources to a non-directory target\n";
        return res;
    }
    for (const auto& src : sources) {
        std::filesystem::path src_path(src);
        std::filesystem::path dst_path = target_is_dir ? target_path /src_path.filename() : target_path;
        if (std::filesystem::is_directory(src_path) && !recursive){
            res.status_code = FAILURE_VECTOR_OUTPUT;
            res.lines_output.push_back("Cannot copy directory '" + src + "': Use -r for recursive copy");
            continue;
        }
        bool src_is_dir = std::filesystem::is_directory(src_path);
        CpCommand::copy(src_path,dst_path,src_is_dir,res);
    }

    return res;
}