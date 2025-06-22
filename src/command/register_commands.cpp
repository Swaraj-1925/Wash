//
// Created by swaraj on 6/20/25.
//

#include "command_registry.h"

void Tab::register_builtin_commands() {
    Tab::command_map["cd"] = std::make_shared<CdCommand>();
    Tab::command_map["ls"] = std::make_shared<LsCommand>();
    Tab::command_map["echo"] = std::make_shared<EchoCommand>();
    Tab::command_map["mkdir"] = std::make_shared<MkdirCommand>();
    Tab::command_map["touch"] = std::make_shared<TouchCommand>();
    Tab::command_map["rm"] = std::make_shared<RmCommand>();

//    Tab::command_map["rmdir"] = std::make_shared<RmdirCommand>();
    Tab::command_map["cp"] = std::make_shared<CpCommand>();
    Tab::command_map["mv"] = std::make_shared<MvCommand>();
}
