#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"

enum CellType { VOID, WALL, PLAYER, COIN, ENEMY };

struct Cell {
    nccell cell;
    enum CellType type;
};

static const Cell DEFAULT_CELL = {NCCELL_TRIVIAL_INITIALIZER, VOID};

static inline Cell create_cell(utf8_char character, enum Color color, enum CellType type) {
    Cell cell = {NCCELL_CHAR_INITIALIZER(character), type};

    nccell_set_fg_rgb(&cell.cell, color);

    return cell;
}

static inline Cell create_default_cell() { return DEFAULT_CELL; }
