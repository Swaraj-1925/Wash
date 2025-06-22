//
// Created by swaraj on 6/20/25.
//

#include "command/command_registry.h"

void Tab::register_builtin_commands() {
    Tab::command_map["cd"] = std::make_shared<CdCommand>();
    Tab::command_map["ls"] = std::make_shared<LsCommand>();
    Tab::command_map["echo"] = std::make_shared<EchoCommand>();
    Tab::command_map["mkdir"] = std::make_shared<MkdirCommand>();
}
