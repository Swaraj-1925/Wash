//
// Created by swaraj on 6/30/25.
//

#include "cat.h"

Output CatCommand::execute(const std::vector<char *> &args) {
    Output output;

    if (args.size() < 2) {
        output.status_message = "cat: missing file operand";
        output.error_details.push_back("Usage: cat [FILE]...");
        output.status_code = FAILURE_INVALID_INPUT;
        return output;
    }
    std::ostringstream content;
    bool has_error = false;
    for (size_t i = 1; i < args.size() && args[i] != nullptr; ++i) {
        std::string filename = args[i];

        // Check if file exists
        if (!std::filesystem::exists(filename)) {
            output.error_details.push_back("cat: " + filename + ": No such file or directory");
            has_error = true;
            continue;
        }

        // Check if it's a regular file
        if (!std::filesystem::is_regular_file(filename)) {
            output.error_details.push_back("cat: " + filename + ": Is a directory");
            has_error = true;
            continue;
        }

        // Try to open and read the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            output.error_details.push_back("cat: " + filename + ": Permission denied");
            has_error = true;
            continue;
        }

        // Read file content line by line
        std::string line;
        while (std::getline(file, line)) {
            content << line << "\n";
            output.lines_output.push_back(line);
        }

        file.close();
    }

    // Set output content
    output.text_output = content.str();

    // Set status based on whether we had any errors
    if (has_error) {
        output.status_code = FAILURE;
        if (output.lines_output.empty()) {
            output.status_message = "cat: failed to read any files";
        } else {
            output.status_code = SUCCESS; // If you have this status
            output.status_message = "cat: some files could not be read";
        }
    } else {
        output.status_code = SUCCESS;
    }

    return output;
}

