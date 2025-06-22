//
// Created by swaraj on 6/22/25.
//

#ifndef WASH_TOUCH_H
#define WASH_TOUCH_H

#include "command.h"
class TouchCommand :public Command{

public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override {return 0;}
};


#endif //WASH_TOUCH_H
