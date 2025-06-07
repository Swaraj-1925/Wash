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
    // Background layers
    ThemeColor TERM_BG= ThemeColor(0x181926);
    ThemeColor STATUS_BG = ThemeColor(0xdf8e1d);
    ThemeColor TEXT1 = ThemeColor(0xa5adcb);
};

// Default LIGHT theme of terminal inspired by
#endif //WASH_THEME_H
