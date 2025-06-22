//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_MV_H
#define WASH_MV_H

#include "command.h"
class MvCommand : public Command{
private:
    bool force = false;
    bool verbose = false;
    std::vector<std::string> sources;
    std::string target;

private:
    void parse_args(const std::vector<std::string>& args);
    bool move(const std::filesystem::path& src, const std::filesystem::path& dst, Output& res);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override{return 0;};
};


#endif //WASH_MV_H
