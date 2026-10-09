#pragma once

// Colors: https://wixdaq.github.io/Tokyo-Night-Website/palette.html

#include "types.h"

enum Color {
    BLACK = 0x000000,            // #000000
    RED = 0xF7768E,              // #F7768E
    ORANGE = 0xFF9E64,           // #FF9E64
    YELLOW = 0xE0AF68,           // #E0AF68
    BEIGE = 0xCFC9C2,            // #CFC9C2
    GREEN = 0x9ECE6A,            // #9ECE6A
    TEAL = 0x73DACB,             // #73DACB
    LIGHT_TEAL = 0xB4F9F8,       // #B4F9F8
    SKY_BLUE = 0x2AC3DE,         // #2AC3DE
    LIGHT_BLUE = 0x7DCFFF,       // #7DCFFF
    BLUE = 0x7AA2F7,             // #7AA2F7
    MAGENTA = 0xBB9AF7,          // #BB9AF7
    WHITE = 0xC0CAF5,            // #C0CAF5
    LIGHT_GRAY = 0xA9B1D6,       // #A9B1D6
    GRAY = 0x9AA5CE,             // #9AA5CE
    DARK_GRAY = 0x565F89,        // #565F89
    VERY_DARK_GRAY = 0x414868,   // #414868
    HYPER_DARK_GRAY = 0x1A1B26,  // #1A1B26
};

static inline void break_into_parts(enum Color color, byte *red, byte *green, byte *blue) {
    *red = (color & 0xFF0000U) >> 16U;
    *green = (color & 0x00FF00U) >> 8U;
    *blue = color & 0x0000FFU;
}

static inline enum Color grayscale_to_rgb(float scale) {
    byte parts = (byte)(255.0f * scale);

    return parts << 16U | parts << 8U | parts;
}
