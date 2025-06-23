//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_WHOAMI_H
#define WASH_WHOAMI_H

#include "command.h"

class WhoamiCommand :public Command{
private:
    std::string text;
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override;
};


#endif //WASH_WHOAMI_H
