#pragma once

#include "gameplay/entity.h"
#include "utils/utf8.h"

static const utf8_char PLAYER_CHARACTER = UTF8("■");
static const Color PLAYER_COLOR = BLUE;
static const float PLAYER_SPEED = 20.0f;
static const float ENEMY_SPEED = 15.0f;

static inline Entity create_player_entity(Cell *cell, Vector position) {
    return create_entity(cell, position, create_cell(PLAYER_CHARACTER, PLAYER_COLOR, PLAYER));
}

void handle_player_input(Entity *player, InputState *input_state);
