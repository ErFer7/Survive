#pragma once

#include "constants.h"
#include "gameplay/cell.h"
#include "types.h"
#include "utils/vector.h"

struct Entity {
    Cell *cell;
    Vector position;
    float movement_acumulator;
    Vector direction;
};

static inline Entity create_entity(Cell *cell_ref, Vector position, const Cell cell) {
    *cell_ref = cell;

    Entity entity = {cell_ref, position, 0.0f, VECTOR_ZERO};

    return entity;
}

static inline Vector get_direction(Entity *entity) { return entity->direction; }

static inline void set_direction(Entity *entity, Vector direction) { entity->direction = direction; }

// Speed: cells/s
static inline void accumulate_movement(Entity *entity, const float speed) {
    entity->movement_acumulator += (1.0f / (float)UPDATE_FREQUENCY) * speed;
}

static inline bool check_and_reset_movement_accumulator(Entity *entity) {
    if (entity->movement_acumulator > 1.0f) {
        entity->movement_acumulator = 0.0f;

        return true;
    }

    return false;
}

static inline void move_entity(Entity *entity, Cell *new_cell, Vector new_position) {
    entity->position = new_position;
    entity->direction = VECTOR_ZERO;
    *new_cell = *entity->cell;
    *entity->cell = DEFAULT_CELL;
    entity->cell = new_cell;
}
