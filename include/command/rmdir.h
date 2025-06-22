//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_RMDIR_H
#define WASH_RMDIR_H

#include "command.h"
class RmdirCommand : public Command{
private:
    bool verbose = false;
    bool parents = false;
    bool ignore_fail_on_non_empty = false;
    std::vector<std::string> directories;

private:
    void parse_args(const std::vector<std::string>& args);
    bool remove_dir(const std::filesystem::path dir,Output &res);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override{return 0;};
};


#endif //WASH_RMDIR_H
