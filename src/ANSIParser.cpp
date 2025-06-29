//
// Created by swaraj on 6/29/25.
//

#include "ANSIParser.h"
ANSIParser::rgb ANSIParser::ansiToRGB(const std::string& ansi_code){
// Extract the numeric part from codes like "33m", "31m", etc.
    std::string code = ansi_code;
    if (code.back() == 'm') {
        code = code.substr(0, code.length() - 1);
    }
    size_t pos = ansi_code.find(';');
    if (pos != std::string::npos) {
        code = ansi_code.substr(pos + 1);
    }

    int color_num = std::stoi(code);

    // Standard ANSI color mappings
    switch (color_num) {
        case 30: return rgb(30, 30, 46);         // Black
        case 31: return rgb(243, 139, 168);       // Red
        case 32: return rgb(166, 227, 161);       // Green
        case 33: return rgb(229, 200, 144);     // Yellow
        case 34: return rgb(137, 180, 250);       // Blue
        case 35: return rgb(245, 194, 231);     // Magenta
        case 36: return rgb(137, 220, 235);     // Cyan
        case 37: return rgb(220, 224, 232);   // White
        case 90: return rgb(147, 153, 178);   // Bright Black (Gray)
        case 91: return rgb(210, 15, 57);   // Bright Red
        case 92: return rgb(64, 160, 43);   // Bright Green
        case 93: return rgb(223, 142, 29);   // Bright Yellow
        case 94: return rgb(30, 102, 245);   // Bright Blue
        case 95: return rgb(234, 118, 203);   // Bright Magenta
        case 96: return rgb(4, 165, 229);   // Bright Cyan
        case 97: return rgb(220, 224, 232);   // Bright White
        default: return rgb(202, 211, 245);   // Default white
    }
}

std::vector<ColoredSegment> ANSIParser::parseANSI(const std::string& input) {
    std::vector<ColoredSegment> segments;
    std::string current_color = "";

    size_t pos = 0;
    while (pos < input.length()) {
        size_t ansi_start = std::string::npos;

        // Try different ANSI sequence patterns
        // Pattern 1: ^[[
        size_t pattern1 = input.find("^[[", pos);
        // Pattern 2: ESC[ (actual escape character)
        size_t pattern2 = input.find("\x1b[", pos);
        // Pattern 3: \033[
        size_t pattern3 = input.find("\033[", pos);

        // Find the earliest match
        ansi_start = std::min({pattern1, pattern2, pattern3});

        if (ansi_start == std::string::npos) {
            // No more ANSI codes, add remaining text
            if (pos < input.length()) {
                std::string remaining = input.substr(pos);
                if (!remaining.empty()) {
                    segments.emplace_back(remaining, current_color, !current_color.empty());
                }
            }
            break;
        }

        // check for end ANSI
        if (ansi_start > pos) {
            std::string text_before = input.substr(pos, ansi_start - pos);
            if (!text_before.empty()) {
                segments.emplace_back(text_before, current_color, !current_color.empty());
            }
        }

        // Determine which pattern matched and skip accordingly
        size_t skip_chars = 0;
        if (ansi_start == pattern1) skip_chars = 3; // Skip "^[["
        else if (ansi_start == pattern2) skip_chars = 2; // Skip "\x1b["
        else if (ansi_start == pattern3) skip_chars = 4; // Skip "\033["

        size_t ansi_end = input.find('m', ansi_start + skip_chars);
        if (ansi_end == std::string::npos) {
            // Malformed ANSI sequence, treat rest as regular text
            std::string remaining = input.substr(ansi_start);
            if (!remaining.empty()) {
                segments.emplace_back(remaining, current_color, !current_color.empty());
            }
            break;
        }

        // Extract the color code
        std::string color_code = input.substr(ansi_start + skip_chars, ansi_end - ansi_start - skip_chars);

        // Update current color
        if (color_code.empty() || color_code == "0") {
            current_color = "";
        } else {
            current_color = color_code + "m";
        }

        pos = ansi_end + 1;
    }

    return segments;
}

// Get cleaned text without ANSI codes
std::string ANSIParser::getCleanText(const std::string& input) {
    std::string result = input;

    // Remove ^[[...m sequences
    size_t pos = 0;
    while ((pos = result.find("^[[", pos)) != std::string::npos) {
        size_t end = result.find('m', pos);
        if (end != std::string::npos) {
            result.erase(pos, end - pos + 1);
        } else {
            break;
        }
    }

    return result;
}