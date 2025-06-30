//
// Created by swaraj on 6/28/25.
//
#include "auto_completion.h"

TabCompletion::TabCompletion(std::vector<std::string> &debug) : m_debug(debug) {
    auto pathDirs = getPathDirectories();
    for (const auto &dir: pathDirs) {
        try {
            if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
                for (const auto &entry: std::filesystem::directory_iterator(dir)) {
                    if (entry.is_regular_file()) {
                        std::string filename = entry.path().filename().string();
                        TabCompletion::path_commands.insert(filename);
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error &) {
            // Skip directories we can't access
            continue;
        }
    }
    TabCompletion::commands = {"ls", "cd", "cat", "echo", "pwd", "mkdir", "rmdir",
                               "rm", "cp", "mv", "grep", "find", "which", "whereis",
                               "ps", "kill", "top", "chmod", "chown", "tar", "zip",
                               "unzip", "wget", "curl", "git", "make", "gcc", "g++"};
    subcommands["git"] = {
            "add", "branch", "checkout", "clone", "commit", "diff", "fetch", "grep",
            "init", "log", "merge", "mv", "pull", "push", "rebase", "reflog", "remote",
            "reset", "rm", "show", "status", "tag"
    };
    subcommands["docker"] = {
            "build", "commit", "create", "exec", "images", "info", "inspect", "kill",
            "logs", "ps", "pull", "push", "restart", "rm", "rmi", "run", "start", "stop",
            "tag", "version"
    };
}

std::vector<std::string> TabCompletion::getPathDirectories() {
    std::vector<std::string> paths;
    const char *pathEnv = std::getenv("PATH");
    if (!pathEnv) return paths;

    std::string pathStr(pathEnv);
    std::stringstream ss(pathStr);
    std::string path;

    while (std::getline(ss, path, ':')) {
        if (!path.empty()) {
            paths.push_back(path);
        }
    }
    return paths;
}

std::vector<std::string> TabCompletion::getExecutableCommands(const std::string &prefix) {
    std::vector<std::string> matches;
    for (const auto &cmd: TabCompletion::commands) {
        if (cmd.starts_with(prefix)) {
            matches.push_back(cmd);
        }
    }
    if (matches.empty()) {
        for (const auto &cmd: TabCompletion::path_commands) {
            if (cmd.starts_with(prefix)) {
                matches.push_back(cmd);
            }
        }
    }
    std::sort(matches.begin(), matches.end());
    matches.erase(std::unique(matches.begin(), matches.end()), matches.end());
    return matches;
}

std::vector<std::string> TabCompletion::completeFilePath(const std::string &partial) {
    std::vector<std::string> matches;
    std::filesystem::path inputPath(partial);
    std::filesystem::path directory;
    std::string filename;

    if (partial.empty() || partial.back() == '/') {
        directory = partial.empty() ? std::filesystem::current_path() : std::filesystem::path(partial);
        filename = "";
    } else {
        directory = inputPath.parent_path();
        filename = inputPath.filename().string();

        if (directory.empty()) directory = std::filesystem::current_path();
    }
    try {
        if (std::filesystem::exists(directory) && std::filesystem::is_directory(directory)) {
            for (const auto &entry: std::filesystem::directory_iterator(directory)) {
                std::string entryName = entry.path().filename().string();

                if (filename.empty() || entryName.starts_with(filename)) {
                    std::string resultPath;
                    if (directory == std::filesystem::current_path()) {
                        resultPath = entryName;
                    } else {
                        resultPath = (directory / entryName).string();
                    }
                    if (entry.is_directory()) {
                        resultPath += "/";
                    }
                    matches.push_back(resultPath);
                }
            }
        }
    } catch (const std::filesystem::filesystem_error &) {
        // Return empty if we can't access the directory
    }

    std::sort(matches.begin(), matches.end());
    return matches;
}



std::vector<std::string> TabCompletion::getCompletions(const std::string &line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (ss >> token) {
        m_debug.push_back(std::string("token in auto complete: ") + token);
        tokens.push_back(token);
    }
    if (tokens.empty()) {
        return {};
    }

    std::string lastToken = tokens.back();
    if (tokens.size() == 1) {
        // Complete as a command
        return getExecutableCommands(lastToken);
    } else if (tokens.size() == 2 && subcommands.find(tokens[0]) != subcommands.end()) {
        // Complete as a subcommand
        std::vector<std::string> matches;
        const auto& subs = subcommands[tokens[0]];
        for (const auto& sub : subs) {
            if (sub.starts_with(lastToken)) {
                matches.push_back(sub);
            }
        }
        return matches;
    } else {
        // Complete as a file path (or options in future enhancements)
        return completeFilePath(lastToken);
    }
}
std::string TabCompletion::getCommonPrefix(const std::vector<std::string>& matches) {
    if (matches.empty()) return "";
    if (matches.size() == 1) return matches[0];

    std::string prefix = matches[0];
    for (size_t i = 1; i < matches.size(); ++i) {
        size_t j = 0;
        while (j < prefix.length() && j < matches[i].length() &&
               prefix[j] == matches[i][j]) {
            ++j;
        }
        prefix = prefix.substr(0, j);
    }
    return prefix;
}
std::vector<std::string> TabCompletion::handleTabCompletion(std::string input) {
    if (input.empty()) return {};
    auto matches = getCompletions(input);
    if (matches.empty()) {
        m_debug.push_back("match was empty");
    }
    return matches;
}
std::string TabCompletion::replaceLastToken(const std::string& input, const std::string& replacement) {
    if (input.empty()) return replacement;

    // Find the last non-whitespace character
    size_t lastNonSpace = input.find_last_not_of(" \t");
    if (lastNonSpace == std::string::npos) {
        // Input is all whitespace, just append
        return input + replacement;
    }

    // Find the start of the last token
    size_t lastTokenStart = input.find_last_of(" \t", lastNonSpace);
    if (lastTokenStart == std::string::npos) {
        // No spaces found, replace entire string
        return replacement;
    } else {
        // Replace from after the last space
        return input.substr(0, lastTokenStart + 1) + replacement;
    }
}













