//
// Created by swaraj on 6/20/25.
//

#include "command_registry.h"

void Tab::register_builtin_commands() {
    Tab::command_map["cd"] = std::make_shared<CdCommand>();
    Tab::command_map["ls"] = std::make_shared<LsCommand>();
    Tab::command_map["cat"] = std::make_shared<CatCommand>();
}
