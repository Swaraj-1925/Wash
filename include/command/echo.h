//
// Created by swaraj on 6/22/25.
//

#ifndef WASH_ECHO_H
#define WASH_ECHO_H
#include "command.h"
#include <cstdlib>

class EchoCommand : public Command{
private:
    std::string text;
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override;
};
#endif //WASH_ECHO_H
