#pragma once

#include "types.h"
#include "utils/color.h"

struct Cell {
    char character;
    Color color;
};

static inline void init_cell(Cell *cell, char character, Color color) {
    cell->character = character;
    cell->color = color;
}
