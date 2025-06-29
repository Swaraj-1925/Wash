//
// Created by swaraj on 6/20/25.
//

#ifndef WASH_CD_H
#define WASH_CD_H
#include "command.h"

class CdCommand : public Command{
private:
    bool use_physical = false;
    bool check_exists = false;
    bool invalid = false;
    std::string path;
    void parse_args(const std::vector<char *>& args);
public:
    Output execute(const std::vector<char *>& args) override;
    int render_output(ncpp::Plane* plane,int line) override {return 0;}
    void parse_output(const std::vector<char *>& args,std::vector<char *> &exec_args) override {};
};

#endif //WASH_CD_H
