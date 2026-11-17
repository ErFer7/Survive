#include "gameplay/world.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#include "gameplay/cell.h"
#include "stdlib.h"
#include "utils/perlin_noise.h"
#include "utils/random.h"

void init_world(World *world) {
    init_cell_matrix(&world->cell_matrix);
    world->has_terrain = false;
}

void generate_world(World *world, VectorU size, bool enable_terrain_generation) {
    world->has_terrain = enable_terrain_generation;

    allocate_cell_matrix(&world->cell_matrix, size);
    fill_cell_matrix(&world->cell_matrix, DEFAULT_CELL);

    if (enable_terrain_generation) {
        generate_terrain(world);
    }

    generate_walls(world);
    clear_spawn(world);

    generate_coins(world);
    generate_powerups(world);
}

void generate_walls(World *world) {
    VectorU size = world->cell_matrix.size;

    for (uint32_t row = 0; row < size.y; row++) {
        create_wall(world, VECTOR_U(0, row), OPAQUE_WALL_CHARACTER);
        create_wall(world, VECTOR_U(world->cell_matrix.size.x - 1, row), OPAQUE_WALL_CHARACTER);
    }

    for (uint32_t columns = 0; columns < size.x; columns++) {
        create_wall(world, VECTOR_U(columns, 0), OPAQUE_WALL_CHARACTER);
        create_wall(world, VECTOR_U(columns, world->cell_matrix.size.y - 1), OPAQUE_WALL_CHARACTER);
    }
}

void generate_terrain(World *world) {
    int32_t seed = rand();
    VectorU size = world->cell_matrix.size;

#pragma omp parallel for collapse(2)
    for (uint32_t row = 0; row < size.y; row++) {
        for (uint32_t column = 0; column < size.x; column++) {
            VectorU position = VECTOR_U(column, row);
            float noise = perlin_noise((float)column * WORLD_PERLIN_NOISE_SCALE,
                                       (float)row * WORLD_PERLIN_NOISE_SCALE,
                                       WORLD_PERLIN_NOISE_PERSISTENCE,
                                       WORLD_PERLIN_NOISE_OCTAVES,
                                       seed);

            if (noise >= WORLD_MIN_NOISE_THRESHOLD) {
                if (noise <= OPAQUE_TERRAIN_NOISE_THRESHOLD) {
                    create_terrain_wall(world, position, get_terrain_character(noise), get_terrain_color(noise));
                } else {
                    create_terrain_wall(world, position, OPAQUE_WALL_CHARACTER, WALL_COLOR);
                }
            }
        }
    }
}

void clear_spawn(World *world) {
    VectorU center = divide_vector_u_by_scalar(world->cell_matrix.size, 2);

    for (uint32_t row = center.y - SPAWN_RADIUS; row < center.y + SPAWN_RADIUS; row++) {
        for (uint32_t column = center.x - SPAWN_RADIUS; column < center.x + SPAWN_RADIUS; column++) {
            if (vector_u_distance(VECTOR_U(column, row), center) <= SPAWN_RADIUS) {
                set_cell_xy(&world->cell_matrix, row, column, DEFAULT_CELL);
            }
        }
    }
}

Vector get_random_empty_position(World *world, bool avoid_point, Vector avoidance_point, float avoidance_distance) {
    VectorU size = world->cell_matrix.size;

    while (true) {
        Vector position = VECTOR(inclusive_random(1, size.x - 2), inclusive_random(1, size.y - 2));

        if (!avoid_point || vector_distance(avoidance_point, position) > avoidance_distance) {
            Cell *cell = get_cell_ptr_vec(&world->cell_matrix, position);

            if (cell->type == VOID) {
                return position;
            }
        }
    }
}

// TODO: These functions are quite similar, they could be refactored and unified
void generate_coin(World *world) {
    Vector position = get_random_empty_position(world, false, VECTOR_ZERO, 0.0);
    create_coin(world, position);
}

void generate_powerup(World *world) {
    Vector position = get_random_empty_position(world, false, VECTOR_ZERO, 0.0);
    create_powerup(world, position);
}

void generate_coins(World *world) {
    for (uint32_t i = 0;
         i < ceilf((float)(world->cell_matrix.size.x * world->cell_matrix.size.y) / COIN_GENERATION_COEFFICIENT);
         i++) {
        generate_coin(world);
    }
}

void generate_powerups(World *world) {
    for (uint32_t i = 0;
         i < ceilf((float)(world->cell_matrix.size.x * world->cell_matrix.size.y) / POWERUP_GENERATION_COEFFICIENT);
         i++) {
        generate_powerup(world);
    }
}

void free_world(World *world) { free_cell_matrix(&world->cell_matrix); }
