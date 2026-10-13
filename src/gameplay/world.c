#include "gameplay/world.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#include "gameplay/cell.h"
#include "stdlib.h"
#include "utils/perlin_noise.h"
#include "utils/random.h"

void init_world(World *world);

void init_world(World *world) {
    world->size = VECTORU_ZERO;
    world->has_terrain = false;
    world->matrix = nullptr;
}

void generate_world(World *world, VectorU size, bool enable_terrain_generation) {
    world->size = size;
    world->has_terrain = enable_terrain_generation;

    size_t raw_size = sizeof(Cell) * size.x * size.y;

    world->matrix = malloc(raw_size);

    for (uint32_t i = 0; i < size.x * size.y; i++) {
        set_cell_i(world, i, create_default_cell());
    }

    if (enable_terrain_generation) {
        generate_terrain(world);
    }

    generate_walls(world);
    clear_spawn(world);

    generate_coins(world);
}

void generate_walls(World *world) {
    for (uint32_t row = 0; row < world->size.y; row++) {
        create_wall(world, create_vector_u(0, row), OPAQUE_WALL_CHARACTER);
        create_wall(world, create_vector_u(world->size.x - 1, row), OPAQUE_WALL_CHARACTER);
    }

    for (uint32_t columns = 0; columns < world->size.x; columns++) {
        create_wall(world, create_vector_u(columns, 0), OPAQUE_WALL_CHARACTER);
        create_wall(world, create_vector_u(columns, world->size.y - 1), OPAQUE_WALL_CHARACTER);
    }
}

void generate_terrain(World *world) {
    int32_t seed = rand();

#pragma omp parallel for collapse(2)
    for (uint32_t row = 0; row < world->size.y; row++) {
        for (uint32_t column = 0; column < world->size.x; column++) {
            VectorU position = create_vector_u(column, row);
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
    VectorU center = divide_vector_u_by_scalar(world->size, 2);

    for (uint32_t row = center.y - SPAWN_RADIUS; row < center.y + SPAWN_RADIUS; row++) {
        for (uint32_t column = center.x - SPAWN_RADIUS; column < center.x + SPAWN_RADIUS; column++) {
            if (vector_u_distance(create_vector_u(column, row), center) <= SPAWN_RADIUS) {
                set_cell_xy(world, row, column, DEFAULT_CELL);
            }
        }
    }
}

void generate_coin(World *world) {
    while (true) {
        Vector position = create_vector(inclusive_random(1, world->size.x - 2), inclusive_random(1, world->size.y - 2));

        Cell *cell = get_cell_ref_vec(world, position);

        if (cell->type == VOID) {
            create_coin(world, position);
            break;
        }
    }
}

void generate_coins(World *world) {
    for (uint32_t i = 0; i < ceilf((float)(world->size.x * world->size.y) / COIN_GENERATION_COEFFICIENT); i++) {
        generate_coin(world);
    }
}

void free_world(World *world) {
    if (world->matrix != nullptr) {
        free(world->matrix);
        world->matrix = nullptr;
    }
}
