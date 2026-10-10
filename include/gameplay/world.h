#pragma once

#include "cell.h"
#include "utils/normalization.h"
#include "utils/utf8.h"
#include "utils/vector.h"

static const utf8_char OPAQUE_WALL_CHARACTER = UTF8("█");
static const utf8_char DARK_WALL_CHARACTER = UTF8("▓");
static const utf8_char MEDIUM_WALL_CHARACTER = UTF8("▒");
static const utf8_char FAINT_WALL_CHARACTER = UTF8("░");
static const enum Color WALL_COLOR = WHITE;

static const utf8_char COIN_CHARACTER = UTF8("◈");
static const enum Color COIN_COLOR = YELLOW;

static const uint32_t SPAWN_RADIUS = 16;

static const float COIN_DENSITY = 3600.0f;

static const float WORLD_PERLIN_NOISE_SCALE = 0.1f;
static const float WORLD_PERLIN_NOISE_PERSISTENCE = 0.65f;
static const int32_t WORLD_PERLIN_NOISE_OCTAVES = 5;
static const float WORLD_MIN_NOISE_THRESHOLD = 0.7f;
static const float OPAQUE_TERRAIN_NOISE_THRESHOLD = 1.5f;
static const utf8_char WALLS[4] = {FAINT_WALL_CHARACTER,
                                   MEDIUM_WALL_CHARACTER,
                                   DARK_WALL_CHARACTER,
                                   OPAQUE_WALL_CHARACTER};

struct World {
    Cell *matrix;
    VectorU size;
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

static inline Cell *get_cell_ref_xy(World *world, uint32_t row, uint32_t column) {
    return &world->matrix[world->size.x * row + column];
}

static inline void set_cell_xy(World *world, uint32_t row, uint32_t column, Cell cell) {
    world->matrix[world->size.x * row + column] = cell;
}

static inline Cell *get_cell_ref_vec(World *world, VectorU position) {
    return &world->matrix[world->size.x * position.y + position.x];

    return nullptr;
}

static inline void set_cell_vec(World *world, VectorU position, Cell cell) {
    world->matrix[world->size.x * position.y + position.x] = cell;
}

static inline Cell *get_cell_ref_i(World *world, uint32_t index) { return &world->matrix[index]; }

static inline void set_cell_i(World *world, uint32_t index, Cell cell) { world->matrix[index] = cell; }

static inline void create_wall(World *world, VectorU position, const utf8_char wall_character) {
    set_cell_vec(world, position, create_cell(wall_character, WALL_COLOR, WALL));
}

static inline void create_terrain_wall(World *world,
                                       VectorU position,
                                       const utf8_char wall_character,
                                       enum Color color) {
    set_cell_vec(world, position, create_cell(wall_character, color, WALL));
}

static inline void create_coin(World *world, VectorU position) {
    set_cell_vec(world, position, create_cell(COIN_CHARACTER, COIN_COLOR, COIN));
}

static inline utf8_char get_terrain_character(float noise) {
    return WALLS[(uint32_t)(min_max(noise, WORLD_MIN_NOISE_THRESHOLD, OPAQUE_TERRAIN_NOISE_THRESHOLD) * 4.0f)];
}

static inline enum Color get_terrain_color(float noise) {
    return gradient(min_max(noise, WORLD_MIN_NOISE_THRESHOLD, OPAQUE_TERRAIN_NOISE_THRESHOLD), DARK_GRAY, WHITE);
}
