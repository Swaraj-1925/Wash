//
// Created by swaraj on 6/30/25.
//

#ifndef WASH_CAT_H
#define WASH_CAT_H
#include "command.h"
#include <fstream>

class CatCommand : public Command{
private:
    std::string result;
public:
    Output execute(const std::vector<char *>& args) override;
    int render_output(ncpp::Plane* plane,int line) override {return 0;}
    void parse_output(const std::vector<char *>& args,std::vector<char *> &exec_args) override {};
};
#endif //WASH_CAT_H
