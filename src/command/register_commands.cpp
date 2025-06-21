//
// Created by swaraj on 6/20/25.
//

#include "command/command_registry.h"

void Tab::register_builtin_commands() {
    Tab::command_map["cd"] = std::make_shared<CdCommand>();
}
