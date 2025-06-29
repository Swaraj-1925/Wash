//
// Created by swaraj on 6/13/25.
//

#ifndef WASH_COMMAND_H
#define WASH_COMMAND_H
#include <iostream>
#include <vector>
#include <ncpp/NotCurses.hh>
#include <pwd.h>
#include <unistd.h>
#include <variant>
#include <string>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <unordered_set>

#include "constants.h"
#include "theme.h"

struct FileInfo {
    std::string name ; // name of file
    uint32_t num_hardlinks = 0; // number of directory inside director
    std::string size = "0"; // size of file
    std::time_t mtime; // last modified time
    std::string permissions = "----------";
    std::string owner = "";
    std::string group = "";
    std::string type = "";

};
struct Output {
    StatusCode status_code = SUCCESS;
    std::string status_message = "";

    std::vector<std::string> error_details = {};       // List of specific error messages (for debugging/logging)

    std::string text_output = "";                      // Output if it’s a single string (e.g., version, help)
    std::vector<std::string> lines_output = {};        // Output if multiple lines or items (e.g., list of names)
    std::vector<FileInfo> files_output = {};           // Output if listing file info (e.g., from `ls`)
};
class Command  {
public:
    virtual ~Command() = default;
    virtual Output execute(const std::vector<char *>& args) = 0;
    virtual int render_output(ncpp::Plane* plane,int line) = 0;
    virtual void parse_output(const std::vector<char *>& output,std::vector<char *> &exec_args) = 0;
};




#endif //WASH_COMMAND_H
