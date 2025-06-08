//
// Created by swaraj on 6/7/25.
//

#ifndef WASH_THEME_H
#define WASH_THEME_H
#include <cstdint>
struct ThemeColor {
    uint32_t hex;
    ThemeColor(uint32_t value) : hex(value) {}

    uint8_t get_r() const { return (hex >> 16) & 0xFF; } // Extract red (1e)
    uint8_t get_g() const { return (hex >> 8) & 0xFF; }  // Extract green (20)
    uint8_t get_b() const { return hex & 0xFF; }         // Extract blue (30)
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
    ThemeColor TEXT1 = ThemeColor(0xcad3f5);
};

// Default LIGHT theme of terminal inspired by
#endif //WASH_THEME_H
