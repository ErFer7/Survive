#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"

static const nccell DEFAULT_CELL = NCCELL_TRIVIAL_INITIALIZER;

static inline nccell create_cell(utf8_char character, Color color) {
    nccell cell = NCCELL_CHAR_INITIALIZER(character);

    nccell_set_fg_rgb(&cell, color);

    return cell;
}

static inline nccell create_default_cell() { return DEFAULT_CELL; }
