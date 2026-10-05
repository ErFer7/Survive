#pragma once

#include "gameplay/entity.h"

static const Cell PLAYER_CELL = {'@', BLUE};
static const float PLAYER_SPEED = 20.0f;

static inline Entity create_player_entity(Cell *cell, Vector position) {
    return create_entity(cell, position, PLAYER_CELL);
}

void handle_player_input(Entity *player, int key_a, int key_b);
