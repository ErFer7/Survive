#pragma once

// Colors: https://wixdaq.github.io/Tokyo-Night-Website/palette.html

#include "types.h"

enum Color {
    BLACK = 0x000000U,            // #000000
    BRIGHT_CYAN = 0x00FFFFU,      // #00FFFF
    RED = 0xF7768EU,              // #F7768E
    ORANGE = 0xFF9E64U,           // #FF9E64
    YELLOW = 0xE0AF68U,           // #E0AF68
    BEIGE = 0xCFC9C2U,            // #CFC9C2
    GREEN = 0x9ECE6AU,            // #9ECE6A
    TEAL = 0x73DACBU,             // #73DACB
    LIGHT_TEAL = 0xB4F9F8U,       // #B4F9F8
    SKY_BLUE = 0x2AC3DEU,         // #2AC3DE
    LIGHT_BLUE = 0x7DCFFFU,       // #7DCFFF
    BLUE = 0x7AA2F7U,             // #7AA2F7
    MAGENTA = 0xBB9AF7U,          // #BB9AF7
    WHITE = 0xC0CAF5U,            // #C0CAF5
    LIGHT_GRAY = 0xA9B1D6U,       // #A9B1D6
    GRAY = 0x9AA5CEU,             // #9AA5CE
    DARK_GRAY = 0x565F89U,        // #565F89
    VERY_DARK_GRAY = 0x414868U,   // #414868
    HYPER_DARK_GRAY = 0x1A1B26U,  // #1A1B26
};

static const uint32_t RED_MASK = 0xFF0000U;
static const uint32_t GREEN_MASK = 0x00FF00U;
static const uint32_t BLUE_MASK = 0x0000FFU;

static inline void break_into_parts(enum Color color, byte *red, byte *green, byte *blue) {
    *red = (color & RED_MASK) >> 16U;
    *green = (color & GREEN_MASK) >> 8U;
    *blue = color & BLUE_MASK;
}

static inline enum Color grayscale_to_rgb(float scale) {
    byte parts = (byte)(255.0f * scale);

    return parts << 16U | parts << 8U | parts;
}

static inline enum Color gradient(float sample, enum Color start, enum Color end) {
    byte start_red;
    byte start_green;
    byte start_blue;

    byte end_red;
    byte end_green;
    byte end_blue;

    break_into_parts(start, &start_red, &start_green, &start_blue);
    break_into_parts(end, &end_red, &end_green, &end_blue);

    float origin_red = (float)start_red;
    float origin_green = (float)start_green;
    float origin_blue = (float)start_blue;

    float result_red = ((float)end_red - origin_red) * sample + origin_red;
    float result_green = ((float)end_green - origin_green) * sample + origin_green;
    float result_blue = ((float)end_blue - origin_blue) * sample + origin_blue;

    return (byte)result_red << 16U | (byte)result_green << 8U | (byte)result_blue;
}
