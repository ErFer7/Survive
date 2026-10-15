#pragma once

#include "gameplay/entity.h"
#include "utils/utf8.h"

static const utf8_char ENEMY_CHARACTERS[2] = {UTF8("▚"), UTF8("▞")};
static const uint32_t ENEMY_ANIMATION_LENGTH = 2;
static const enum Color ENEMY_COLOR = RED;
static const float ENEMY_SPEED = 10.0f;
static const Vector DIRECTIONS[8] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

static inline void init_enemy_entity(Entity *entity, Cell *cell, Vector position) {
    init_cell(cell, ENEMY_CHARACTERS, ENEMY_ANIMATION_LENGTH, ENEMY_COLOR, ENEMY);
    init_entity(entity, cell, position);
}
