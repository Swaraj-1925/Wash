//
// Created by swaraj on 6/29/25.
//

#ifndef WASH_ANSIPARSER_H
#define WASH_ANSIPARSER_H
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <ncpp/NotCurses.hh>

// Structure to hold a text segment with its color information
struct ColoredSegment {
    std::string text;
    std::string ansi_code;  // Original ANSI code (e.g., "33m", "31m", "32m")
    bool has_color;


    ColoredSegment(const std::string& t, const std::string& code = "", bool colored = false)
            : text(t), ansi_code(code), has_color(colored) {}
};

class ANSIParser {
public:
    struct rgb {
        int r, g, b;
        rgb(int red = 255, int green = 255, int blue = 255) : r(red), g(green), b(blue) {}
    };
    rgb ansiToRGB(const std::string& ansi_code);
    std::vector<ColoredSegment> parseANSI(const std::string& input);
    std::string getCleanText(const std::string& input);
    int print_formated_output(ncpp::Plane *plane , std::vector<std::string> &debug, std::vector<char* > output,int line);

};
#endif //WASH_ANSIPARSER_H
