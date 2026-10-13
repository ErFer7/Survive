#pragma once

#include "config.h"
#include "gameplay/cell.h"
#include "types.h"
#include "utils/vector.h"

struct Entity {
    Cell *cell;
    Vector position;
    float movement_acumulator;
    float speed_modifier;
    Vector direction;

    // This is only used by the enemy. You might think we are wasting memory here with this "massive" 8 bytes vector,
    // but pretty much 99% of entities are enemies anyway
    Vector previous_position;
};

static inline Entity create_entity(Cell *cell_ref, Vector position, const Cell cell) {
    *cell_ref = cell;

    Entity entity = {cell_ref, position, 0.0f, 1.0f, VECTOR_ZERO, VECTOR_ZERO};

    return entity;
}

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
    entity->previous_position = entity->position;
    entity->position = new_position;
    entity->direction = VECTOR_ZERO;
    *new_cell = *entity->cell;
    *entity->cell = DEFAULT_CELL;
    entity->cell = new_cell;
}
