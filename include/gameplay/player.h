#pragma once

#include "gameplay/entity.h"
#include "utils/utf8.h"

static const utf8_char PLAYER_CHARACTER[4] = {UTF8("▇"), UTF8("▆"), UTF8("▅"), UTF8("▆")};
static const uint32_t PLAYER_ANIMATION_LENGTH = 4;
static const enum Color PLAYER_COLOR = BLUE;
static const float PLAYER_SPEED = 20.0f;
static const float PLAYER_SLOW_SPEED_MODIFIER = 0.5f;
static const float PLAYER_FAST_SPEED_MODIFIER = 1.5f;

void handle_player_input(Entity *player, InputState *input_state);

static inline void init_player_entity(Entity *entity, Cell *cell, Vector position) {
    init_cell(cell, PLAYER_CHARACTER, PLAYER_ANIMATION_LENGTH, PLAYER_COLOR, PLAYER);
    init_entity(entity, cell, position);
}
