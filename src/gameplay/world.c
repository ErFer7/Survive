#include "gameplay/world.h"

#include <math.h>
#include <string.h>

#include "stdlib.h"
#include "utils/perlin_noise.h"
#include "utils/random.h"

void init_world(World *world);

void init_world(World *world) {
    world->size = VECTOR_ZERO;
    world->has_terrain = false;
    world->matrix = nullptr;
}

void generate_world(World *world, Vector size, bool enable_terrain_generation) {
    world->size = size;
    world->has_terrain = enable_terrain_generation;

    size_t raw_size = sizeof(Cell) * size.x * size.y;

    world->matrix = malloc(raw_size);

    for (unsigned int i = 0; i < size.x * size.y; i++) {
        set_cell_i(world, i, create_default_cell());
    }

    if (enable_terrain_generation) {
        generate_terrain(world);
    }

    generate_walls(world);
    generate_coins(world);
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
            float noise = perlin_noise((float)column * 0.1f, (float)row * 0.1f, 0.65f, 5, seed);

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
    for (uint32_t i = 0; i < ceilf((float)(world->size.x * world->size.y) / 3600.0f); i++) {
        generate_coin(world);
    }
}

void free_world(World *world) {
    if (world->matrix != nullptr) {
        free(world->matrix);
        world->matrix = nullptr;
    }
}
