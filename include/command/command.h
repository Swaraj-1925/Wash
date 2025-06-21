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

struct FileInfo {
    std::string name ; // name of file
    bool is_executable = false;
    bool is_symlink = false;
    bool is_dir= false;  // file or dir
    uint32_t num_hardlinks = 0; // number of directory inside director
    std::string size = "0"; // size of file
    std::time_t mtime; // last modified time
    std::string permissions = "----------";
    std::string owner = "";
    std::string group = "";

};
struct Output {
    int status_code = 200;
    std::string message = "";
    std::string stdout_output = "";  // renamed
    std::vector<FileInfo> file_info_output = {};
};
class Command  {
public:
    virtual Output execute(const std::vector<std::string>& args) = 0;
    virtual ~Command() = default;
    virtual int render_output(ncpp::Plane* plane,Output output,int line) = 0;
};




#endif //WASH_COMMAND_H
