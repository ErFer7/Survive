#include "gameplay/world.h"

#include <string.h>

#include "gameplay/entity.h"
#include "gameplay/player.h"
#include "stdlib.h"

void init_world(World *world, Vector size) {
    world->size = size;

    size_t raw_size = sizeof(Cell) * size.x * size.y;

    world->matrix = malloc(raw_size);

    for (unsigned int i = 0; i < size.x * size.y; i++) {
        set_cell_i(world, i, create_default_cell());
    }

    world->coins = nullptr;
    world->enemies = nullptr;
}

void update_entities(World *world) {
    if (is_non_zero(world->player.direction)) {
        accumulate_movement(&world->player, PLAYER_SPEED);
    }

    if (check_and_reset_movement_accumulator(&world->player)) {
        Vector new_position = add_vector(world->player.position, world->player.direction);
        Cell *new_cell = get_cell_ref_vec(world, new_position);

        move_entity(&world->player, new_cell, new_position);
    }
}

void free_world(World *world) {
    if (world->matrix != nullptr) {
        free(world->matrix);
        world->matrix = nullptr;
    }

    if (world->coins != nullptr) {
        free(world->coins);
        world->coins = nullptr;
    }

    if (world->enemies != nullptr) {
        free(world->enemies);
        world->enemies = nullptr;
    }
}
