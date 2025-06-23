//
// Created by swaraj on 6/7/25.
//

#ifndef WASH_THEME_H
#define WASH_THEME_H
#include <cstdint>
struct ThemeColor {
    uint32_t hex;
    ThemeColor(uint32_t value) : hex(value) {}

    uint8_t r() const { return (hex >> 16) & 0xFF; } // Extract red (1e)
    uint8_t g() const { return (hex >> 8) & 0xFF; }  // Extract green (20)
    uint8_t b() const { return hex & 0xFF; }         // Extract blue (30)
};



// Default DARK theme of terminal inspired by Catppuccin Macchiato
// Background layers
struct Theme {
    // Crust layers
    ThemeColor TERM_CRUST_BG= ThemeColor(0x181926);
    ThemeColor TERM_MANTLE_BG = ThemeColor(0x1e2030);
    ThemeColor TERM_BASE_BG = ThemeColor(0x303446);

    ThemeColor SURFACE_BG1 = ThemeColor(0x51576d);
    ThemeColor TEXT0 = ThemeColor(0xcad3f5);
    ThemeColor SUB_TEXT1 = ThemeColor(0xbac2de);
    ThemeColor SUB_TEXT0 = ThemeColor(0xa6adc8);

    //permission
    ThemeColor PEACH = ThemeColor(0xfab387);
    //hyperlinks
    ThemeColor TEAL = ThemeColor(0x94e2d5);
    //user
    ThemeColor YELLOW = ThemeColor(0xf9e2af);
    //group
    ThemeColor GREEN = ThemeColor(0xa6e3a1);
    //time
    ThemeColor MAROON = ThemeColor(0xeba0ac);
    // normal file text 0

    //excutable
    ThemeColor SAPPHIRE = ThemeColor(0x74c7ec);
    //syslink
    ThemeColor BLUE = ThemeColor(0x89b4fa);

    // dot files sub text 0

};

// Default LIGHT theme of terminal inspired by
#endif //WASH_THEME_H
