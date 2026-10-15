#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"

#define MAX_ANIMATION_LENGTH 6

enum CellType { VOID, WALL, PLAYER, COIN, ENEMY };

// 48 bytes
struct Cell {
    utf8_char characters[MAX_ANIMATION_LENGTH];
    nccell cell;
    float animation_index;
    enum CellType type;
};

static const Cell DEFAULT_CELL = {{}, NCCELL_TRIVIAL_INITIALIZER, 0, VOID};

void update_cell_character(Cell *cell);

static inline void init_cell(Cell *cell,
                             const utf8_char *characters,
                             uint32_t animation_length,
                             enum Color color,
                             enum CellType type) {
    memcpy(cell->characters, characters, sizeof(utf8_char) * animation_length);

    cell->cell = (nccell)NCCELL_CHAR_INITIALIZER(characters[0]);
    cell->animation_index = 0.0f;
    cell->type = type;

    nccell_set_fg_rgb(&cell->cell, color);
}
