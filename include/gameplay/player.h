#pragma once

#include "gameplay/entity.h"
#include "utils/utf8.h"

static const utf8_char PLAYER_CHARACTER = UTF8("■");
static const enum Color PLAYER_COLOR = BLUE;
static const float PLAYER_SPEED = 20.0f;

void handle_player_input(Entity *player, InputState *input_state);

static inline Entity create_player_entity(Cell *cell, VectorU position) {
    return create_entity(cell, position, create_cell(PLAYER_CHARACTER, PLAYER_COLOR, PLAYER));
}
