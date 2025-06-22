//
// Created by swaraj on 6/22/25.
//

#ifndef WASH_MKDIR_H
#define WASH_MKDIR_H
#include "command.h"


class MkdirCommand : public Command {
private:
    std::vector<std::string> directories;
    bool parents = false;
    bool verbose = false;
    bool set_mode = false;
    mode_t mode = 0777;

    std::string err;
public:
    void parse_args(const std::vector<std::string>& args);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override;
};
#endif //WASH_MKDIR_H
