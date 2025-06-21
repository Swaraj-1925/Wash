//
// Created by swaraj on 6/21/25.
//

#ifndef WASH_LS_H
#define WASH_LS_H

#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <ncpp/NotCurses.hh>
#include <iomanip>
#include <sstream>

#include "theme.h"
#include "command.h"
#include "options.h"


class LsCommand : public Command {
public:
    LsCommand() ;
private:
    bool show_all = false;    // -a: show hidden files
    bool long_format = false; // -l: long format
    bool human_readable = false; // -h: human-readable sizes
    bool sort_size = false;   // -s: sort by size
    bool sort_time = false;   // -t: sort by time
    bool reverse = false;     // -r: reverse sort
    bool invalid = false;

    std::string path ; // Default to current directory
    std::vector<FileInfo> files;

private:
    void parse_args(const std::vector<std::string>& args);
    void get_long_format(std::filesystem::perms perms,std::string &perm_str);
    void sort_files();
    std::string format_size(uintmax_t size);
public:
    Output execute(const std::vector<std::string>& args) override ;
    ThemeColor get_theme(std::string title);
    int render_output(ncpp::Plane* plane,Output output,int line) override;

};
#endif //WASH_LS_H
