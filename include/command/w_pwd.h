//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_W_PWD_H
#define WASH_W_PWD_H

#include "command.h"
class PwdCommand : public Command{
private:
    std::string text;
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override;
};


#endif //WASH_W_PWD_H
