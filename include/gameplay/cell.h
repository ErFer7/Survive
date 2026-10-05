#pragma once

#include "types.h"
#include "utils/color.h"

struct Cell {
    char character;
    Color color;
};

static const Cell DEFAULT_CELL = {' ', HYPER_DARK_GRAY};

static inline Cell create_cell(char character, Color color) {
    Cell cell = {character, color};

    return cell;
}

static inline Cell create_default_cell() { return DEFAULT_CELL; }
