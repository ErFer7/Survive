#pragma once

#include "cell.h"
#include "gameplay/cell_matrix.h"
#include "utils/normalization.h"
#include "utils/utf8.h"
#include "utils/vector.h"

static const utf8_char WALLS[4] = {UTF8("░"), UTF8("▒"), UTF8("▓"), UTF8("█")};
static const utf8_char OPAQUE_WALL_CHARACTER = WALLS[3];
static const uint32_t WALL_ANIMATION_LENGTH = 1;
static const enum Color WALL_COLOR = WHITE;

static const utf8_char COIN_CHARACTERS[6] = {UTF8("⠇"), UTF8("⠋"), UTF8("⠙"), UTF8("⠸"), UTF8("⠴"), UTF8("⠦")};
static const uint32_t COIN_ANIMATION_LENGTH = 6;
static const enum Color COIN_COLOR = YELLOW;

static const uint32_t SPAWN_RADIUS = 16;

// 1 coin for 3600 cells
static const float COIN_GENERATION_COEFFICIENT = 3600.0f;

static const float WORLD_PERLIN_NOISE_SCALE = 0.1f;
static const float WORLD_PERLIN_NOISE_PERSISTENCE = 0.65f;
static const int32_t WORLD_PERLIN_NOISE_OCTAVES = 5;
static const float WORLD_MIN_NOISE_THRESHOLD = 0.7f;
static const float OPAQUE_TERRAIN_NOISE_THRESHOLD = 1.5f;

struct World {
    CellMatrix cell_matrix;
    bool has_terrain;
};

void init_world(World *world);
void generate_world(World *world, VectorU size, bool enable_terrain_generation);
void generate_walls(World *world);
void generate_terrain(World *world);
void clear_spawn(World *world);
void generate_coin(World *world);
void generate_coins(World *world);
void free_world(World *world);

static inline void create_wall(World *world, VectorU position, const utf8_char wall_character) {
    utf8_char wall_array[1] = {wall_character};
    Cell cell;

    init_cell(&cell, wall_array, WALL_ANIMATION_LENGTH, WALL_COLOR, WALL);
    set_cell_vec(&world->cell_matrix, vector_u_to_vector(position), cell);
}

static inline void create_terrain_wall(World *world,
                                       VectorU position,
                                       const utf8_char wall_character,
                                       enum Color color) {
    utf8_char wall_array[1] = {wall_character};
    Cell cell;

    init_cell(&cell, wall_array, WALL_ANIMATION_LENGTH, color, WALL);
    set_cell_vec(&world->cell_matrix, vector_u_to_vector(position), cell);
}

static inline void create_coin(World *world, Vector position) {
    Cell cell;

    init_cell(&cell, COIN_CHARACTERS, COIN_ANIMATION_LENGTH, COIN_COLOR, COIN);
    set_cell_vec(&world->cell_matrix, position, cell);
}

static inline utf8_char get_terrain_character(float noise) {
    return WALLS[(uint32_t)(min_max(noise, WORLD_MIN_NOISE_THRESHOLD, OPAQUE_TERRAIN_NOISE_THRESHOLD) * 4.0f)];
}

static inline enum Color get_terrain_color(float noise) {
    return gradient(min_max(noise, WORLD_MIN_NOISE_THRESHOLD, OPAQUE_TERRAIN_NOISE_THRESHOLD), DARK_GRAY, WHITE);
}
