//
// Created by swaraj on 6/4/25.
//

#ifndef WASH_HELPER_H
#define WASH_HELPER_H
#include <iostream>

int ctrl_c_press_count = 0;
// Signal handler function for SIGINT (Ctrl+C)
// When you press Ctrl+C in a terminal, the terminal driver sends a SIGINT signal to the foreground process
void handle_ctrl_c(int sig) {
    ++ctrl_c_press_count;
}



#endif //WASH_HELPER_H
