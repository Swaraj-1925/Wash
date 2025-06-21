//
// Created by swaraj on 6/20/25.
//

#ifndef WASH_CD_H
#define WASH_CD_H
#include "command.h"
#include <string>
#include <vector>

class CdCommand : public Command{
private:
    bool use_physical = false;
    bool check_exists = false;
    bool invalid = false;
    std::string path;
private:
    void parse_args(const std::vector<std::string>& args);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override {return 0;}
};

#endif //WASH_CD_H
