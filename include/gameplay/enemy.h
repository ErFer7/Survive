#pragma once

#include "gameplay/entity.h"
#include "utils/utf8.h"

static const utf8_char ENEMY_CHARACTER = UTF8("█");
static const enum Color ENEMY_COLOR = RED;
static const float ENEMY_SPEED = 10.0f;

static inline Entity create_enemy_entity(Cell *cell, VectorU position) {
    return create_entity(cell, position, create_cell(ENEMY_CHARACTER, ENEMY_COLOR, ENEMY));
}
