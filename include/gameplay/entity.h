#pragma once

#include <math.h>

#include "config.h"
#include "entity_list_reference.h"
#include "gameplay/cell.h"
#include "types.h"
#include "utils/vector.h"

struct Entity {
    Cell *cell_ptr;
    Vector position;
    float movement_acumulator;
    float speed_modifier;
    Vector direction;

    // This is only used by the enemy. You might think we are wasting memory here with this "massive" 8 bytes vector,
    // but pretty much 99% of entities are enemies anyway
    Vector previous_position;
    EntityListReference reference;
};

static inline void init_entity(Entity *entity, Cell *cell_ptr, Vector position) {
    entity->cell_ptr = cell_ptr;
    entity->position = position;
    entity->movement_acumulator = 0.0f;
    entity->speed_modifier = 1.0f;
    entity->direction = VECTOR_ZERO;
    entity->previous_position = VECTOR_ZERO;
}

// Speed: cells/s
static inline bool update_and_check_movement_accumulator(Entity *entity, const float speed) {
    if (!vector_is_zero(entity->direction)) {
        entity->movement_acumulator += (1.0f / (float)UPDATE_FREQUENCY) * speed * entity->speed_modifier;
    }

    if (entity->movement_acumulator > 1.0f) {
        entity->movement_acumulator = 0.0f;

        return true;
    }

    return false;
}

// TODO: Check this up
static inline void move_entity(Entity *entity, Cell *new_cell_ptr, Vector new_position) {
    entity->cell_ptr->entity_ptr = nullptr;
    entity->previous_position = entity->position;
    entity->position = new_position;
    entity->direction = VECTOR_ZERO;
    *new_cell_ptr = *entity->cell_ptr;
    *entity->cell_ptr = DEFAULT_CELL;
    entity->cell_ptr = new_cell_ptr;
    new_cell_ptr->entity_ptr = entity;
}

static inline void deactivate_entity(Entity *entity) {
    // The entity will be completely stopped, the idea here is to just stop it for an iteration in the
    // update_entity_movement() in order to delete it later. This will avoid the cost of a validity check for every
    // entity :p
    entity->movement_acumulator = -INFINITY;
}

static inline void set_entity_color(Entity *entity, enum Color color) { set_cell_color(entity->cell_ptr, color); }
