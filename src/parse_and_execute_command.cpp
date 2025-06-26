//
// Created by swaraj on 6/27/25.
//
#include "tab.h"

// create a process using fork then use exec for replacing the current process image (memory, code, and data) with a new process image
// using pipe redirect std i/o to temporary buffer
int Tab::parse_and_execute_command(const std::string &line) {
    std::istringstream iss(line);
    std::string  cmd;
    iss >> cmd;
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) args.push_back(arg);

    std::vector<char*> exec_args;
    exec_args.push_back(const_cast<char*>(cmd.c_str())); // Command
    for (const auto& a : args) exec_args.push_back(const_cast<char*>(a.c_str())); // Arguments
    exec_args.push_back(nullptr);

    // create file descriptor for read and write end of pipe
    // A pipe is a temporary buffer managed by the kernel that connects the output of one process to the input of another. It’s a simple way to pass data between related processes
    // data is read in order they are writen FIFO
    // Data flows in one direction only
    int filedes[2];
    if(pipe(filedes) == -1){
        Tab::errors.push_back(std::string("Failed to create file descriptor: ") + strerror(errno) + "\n");
        return EXIT_FAILURE;
    }

    pid_t fpid = fork();
    int status;
    char buff[1024];
    int output_line = Tab::m_Line + 1;
    switch (fpid) {
        case -1: // error
            close(filedes[0]);
            close(filedes[1]);
            return EXIT_FAILURE;
            break;
        case 0: //child process

            close(filedes[0]); // disable read end

            // Redirect stdout to the pipe
            if (dup2(filedes[1], STDOUT_FILENO) == -1) {
                Tab::errors.push_back(std::string("Error redirecting STD to filedes: ") + strerror(errno) + "\n");
                exit(EXIT_FAILURE); // Exit on redirection failure
            }
            close(filedes[1]); // disable write end
            if (execvp(exec_args[0],exec_args.data()) < 0){
                Tab::errors.push_back(std::string("Error in execvp: ") + strerror(errno) + "\n");
                return EXIT_FAILURE;
            }
            break;
        default: //parent process
            close(filedes[1]); // disable read end
            ssize_t count;
            while ((count = read(filedes[0],buff,sizeof(buff) -1)) > 0){
                buff[count] = '\0';
                char *line_start = buff;
                char *new_line;
                while((new_line = strchr(line_start,'\n')) != nullptr){
                    *new_line = '\0';
                    //print data
                    m_p_Plane->printf(output_line++, NCALIGN_LEFT, "%s", line_start);
                    line_start = new_line +1;
                }
                if (*line_start){
                    m_p_Plane->printf(output_line++, NCALIGN_LEFT, "%s", line_start);
                }
            }
            close(filedes[0]);
            pid_t wpid = waitpid(fpid,&status,0);
            while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
                wpid = waitpid(fpid, &status, 0);    // Keep waiting if not exited or signaled
            }
            if (WEXITSTATUS(status) != 0) {
                Tab::errors.push_back(std::string("Command failed: ") + cmd + "\n");
                return EXIT_FAILURE;
            }
            Tab::m_Line = output_line;
            break;
    }

    return EXIT_SUCCESS;
}