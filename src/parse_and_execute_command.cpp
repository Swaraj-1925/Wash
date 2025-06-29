//
// Created by swaraj on 6/27/25.
//
#include "tab.h"
#include "ANSIParser.h"
std::vector<char*> Tab::parse_command(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    std::vector<char*> argv;

    // pull out only non-empty tokens
    while (iss >> token) {
        char* copy = new char[token.size()+1];
        std::strcpy(copy, token.c_str());
        argv.push_back(copy);

    }
    // execvp wants a terminating nullptr
    if (!argv.empty() && std::strstr(argv[0], "git") && argv.size() >= 2) {
        std::string subcommand = argv[1];

        if (subcommand == "log" || subcommand == "show" || subcommand == "reflog") {
            argv.push_back(strdup("--color"));
            argv.push_back(strdup("--decorate"));
        } else if (subcommand == "diff" || subcommand == "blame") {
            argv.push_back(strdup("--color"));
        }
    }
    argv.push_back(nullptr);
    return argv;
}
// create a process using fork then use exec for replacing the current process image (memory, code, and data) with a new process image
// using pipe redirect std i/o to temporary buffer
int Tab::handle_command(std::vector<char *> &exec_args) {
// create file descriptor for read and write end of pipe
// A pipe is a temporary buffer managed by the kernel that connects the output of one process to the input of another. It’s a simple way to pass data between related processes
// data is read in order they are writen FIFO
// Data flows in one direction only
    int filedes[2];
    if (pipe(filedes) == -1) {
        Tab::errors.push_back(std::string("Failed to create file descriptor: ") + strerror(errno) + "\n");
        return EXIT_FAILURE;
    }

    pid_t fpid = fork();
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
            if (dup2(filedes[1], STDERR_FILENO) == -1) {
                Tab::errors.push_back(std::string("Error redirecting STDERR to filedes: ") + strerror(errno) + "\n");
                exit(EXIT_FAILURE);
            }
            close(filedes[1]); // disable write end
            if (execvp(exec_args[0], exec_args.data()) < 0) {
                Tab::errors.push_back(std::string("Error in execvp: ") + strerror(errno) + "\n");
                exit(EXIT_FAILURE);
            }
            break;
        default: //parent process
            return Tab::execute_command(fpid,filedes,exec_args);
            break;
    }

    return EXIT_SUCCESS;
}

int Tab::execute_command(pid_t fpid, int *filedes,std::vector<char *> &exec_args) {
    int status;
    char buff[1024];
    close(filedes[1]); // disable read end
    ssize_t count;
    std::vector<char *> output;

    while ((count = read(filedes[0], buff, sizeof(buff) - 1)) > 0) {
        buff[count] = '\0';
        char *line_start = buff;
        char *new_line;
//        std::string debug_line = escape_string(std::string(line_start)) + " " + strerror(errno) + "\\n";

        int res = -1;
        while ((res = std::string(buff).find("^", res + 1)) != std::string::npos){
            Tab::debug.push_back("ANSI color codes detected\n");
        }
//        Tab::debug.push_back(debug_line);
        while ((new_line = strchr(line_start, '\n')) != nullptr) {
            *new_line = '\0';
            output.push_back(strdup(line_start));

            line_start = new_line + 1;
        }
        if (*line_start) {
            output.push_back(strdup(line_start));
        }
    }

    auto it = Tab::command_map.find(exec_args[0]);
    if(it != Tab::command_map.end()){
        it->second->parse_output(output,exec_args);
        Tab::m_Line =  it->second->render_output(Tab::m_p_Plane,Tab::m_Line+1);
    } else{
        ANSIParser parser;
        Tab::m_Line = parser.print_formated_output(Tab::m_p_Plane,debug,output,Tab::m_Line+1);
    }
    Tab::m_p_Plane->set_fg_default();

    close(filedes[0]);
    pid_t wpid = waitpid(fpid, &status, 0);
    while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
        wpid = waitpid(fpid, &status, 0);    // Keep waiting if not exited or signaled
    }
    if (WEXITSTATUS(status) != 0) {
        Tab::errors.push_back(std::string("Command failed: ") +exec_args[0]+ "\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}