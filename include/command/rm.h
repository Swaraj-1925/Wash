//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_RM_H
#define WASH_RM_H

#include "command.h"
class RmCommand : public Command{
private:
    bool recursive = false;
    bool force = false;
    std::vector<std::string> files;
private:
    void parse_args(const std::vector<std::string>& args);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override{return 0;};
};


#endif //WASH_RM_H
