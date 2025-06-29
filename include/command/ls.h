//
// Created by swaraj on 6/29/25.
//

#ifndef WASH_LS_H
#define WASH_LS_H
#include "command.h"
class LsCommand :public Command {
private:
    struct FileInfo {
        std::string name;
        std::string permissions;
        std::string owner;
        std::string group;
        std::string size;
        std::string month, day, time_or_year;
        uint32_t num_hardlinks;
        std::string type;
    };
    std::vector<FileInfo> files;
    bool long_format = false;

public:
    Output execute(const std::vector<char *>& args) {return {};}
    ThemeColor get_theme(std::string title);
    int render_output(ncpp::Plane* plane,int line) override;
    void parse_output(const std::vector<char *>& output,std::vector<char *> &exec_args ) override ;
};
#endif //WASH_LS_H
