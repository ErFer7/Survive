#include "gameplay/world.h"

#include <string.h>

#include "gameplay/entity.h"
#include "gameplay/player.h"
#include "stdlib.h"
#include "utils/perlin_noise.h"

void init_world(World *world, Vector size) {
    world->size = size;

    size_t raw_size = sizeof(nccell) * size.x * size.y;

    world->matrix = malloc(raw_size);

    for (unsigned int i = 0; i < size.x * size.y; i++) {
        set_cell_i(world, i, create_default_cell());
    }

    world->coins = nullptr;
    world->enemies = nullptr;

    generate_terrain(world);
    generate_walls(world);

    create_player(world, create_vector(size.x / 2, size.y / 2));
}

void generate_walls(World *world) {
    for (unsigned int row = 0; row < world->size.y; row++) {
        create_wall(world, create_vector(0, row), OPAQUE_WALL_CHARACTER);
        create_wall(world, create_vector(world->size.x - 1, row), OPAQUE_WALL_CHARACTER);
    }

    for (unsigned int columns = 0; columns < world->size.x; columns++) {
        create_wall(world, create_vector(columns, 0), OPAQUE_WALL_CHARACTER);
        create_wall(world, create_vector(columns, world->size.y - 1), OPAQUE_WALL_CHARACTER);
    }
}

void generate_terrain(World *world) {
    int seed = rand();

#pragma omp parallel for collapse(2)
    for (unsigned int row = 0; row < world->size.y; row++) {
        for (unsigned int column = 0; column < world->size.x; column++) {
            Vector position = create_vector(column, row);
            float noise = perlin_noise((float)column * 0.1, (float)row * 0.1, 0.65, 5, seed);

            if (noise > 0.7 && noise <= 0.775) {
                create_wall(world, position, FAINT_WALL_CHARACTER);
            } else if (noise > 0.775 && noise <= 0.85) {
                create_wall(world, position, MEDIUM_WALL_CHARACTER);
            } else if (noise > 0.85 && noise <= 0.925) {
                create_wall(world, position, DARK_WALL_CHARACTER);
            } else if (noise > 0.925) {
                create_wall(world, position, OPAQUE_WALL_CHARACTER);
            }
        }
    }
}

void update_entities(World *world) {
    if (is_non_zero(world->player.direction)) {
        accumulate_movement(&world->player, PLAYER_SPEED);
    }

    if (check_and_reset_movement_accumulator(&world->player)) {
        Vector new_position = add_vector(world->player.position, world->player.direction);
        nccell *new_cell = get_cell_ref_vec(world, new_position);

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
