//
// Created by swaraj on 6/30/25.
//

#ifndef WASH_AUTO_COMPLETION_H
#define WASH_AUTO_COMPLETION_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <filesystem>

class TabCompletion {
private:
    std::unordered_set<std::string> commands;
    std::unordered_set<std::string> path_commands;
    std::string currentInput;
    size_t cursorPos;
    std::vector<std::string> m_debug;
    std::unordered_map<std::string, std::vector<std::string>> subcommands; // Added for subcommands
public:
    struct CompletionResult {
        std::vector<std::string> matches;
        std::string completion;
        bool isComplete;

        CompletionResult() : isComplete(false) {}
        CompletionResult(const std::vector<std::string>& m, const std::string& c, bool complete)
                : matches(m), completion(c), isComplete(complete) {}
    };
public:
    TabCompletion(std::vector<std::string> &debug);
private:
    std::vector<std::string> getPathDirectories();
    std::vector<std::string> getExecutableCommands(const std::string& prefix);
    std::vector<std::string> completeFilePath(const std::string& partial);
    std::vector<std::string> getCompletions(const std::string& line);
    std::vector<std::string> getSubcommandCompletions(const std::vector<std::string>& tokens, const std::string& partial);
    std::string getCommonPrefix(const std::vector<std::string>& matches);
    std::string replaceLastToken(const std::string& input, const std::string& replacement);
public:
    std::vector<std::string> handleTabCompletion(std::string input);
};
#endif //WASH_AUTO_COMPLETION_H
