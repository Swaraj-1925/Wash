//
// Created by swaraj on 6/20/25.
//

#ifndef WASH_CD_H
#define WASH_CD_H
#include "command.h"
#include <string>
#include <vector>

class CdCommand : public Command {
private:
    bool use_physical = false;
    bool check_exists = false;
    std::string path;

public:
    std::string execute(const std::vector<std::string>& args) override;
    void parse_args(const std::vector<std::string>& args) override;
};

#endif //WASH_CD_H
