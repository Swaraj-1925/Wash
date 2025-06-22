//
// Created by swaraj on 6/23/25.
//

#ifndef WASH_CP_H
#define WASH_CP_H

#include "command.h"
class CpCommand : public Command{
private:
    bool recursive = false;
    bool force = false;
    bool verbose = false;
    std::vector<std::string> sources;
    std::string target;
private:
    void parse_args(const std::vector<std::string>& args);
    bool copy(const std::filesystem::path& src, const std::filesystem::path& dst,bool is_src_dir, Output& res);
public:
    Output execute(const std::vector<std::string>& args) override;
    int render_output(ncpp::Plane* plane,Output output,int line) override{return 0;};
};


#endif //WASH_CP_H
