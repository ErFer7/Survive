#pragma once

#include "cell.h"
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

static inline Cell *get_cell_ref_xy(World *world, uint32_t row, uint32_t column) {
    if (row < world->size.y && column < world->size.x) {
        return &world->matrix[world->size.x * row + column];
    }

    return nullptr;
}

static inline void set_cell_xy(World *world, uint32_t row, uint32_t column, Cell cell) {
    world->matrix[world->size.x * row + column] = cell;
}

static inline Cell *get_cell_ref_vec(World *world, VectorU position) {
    if (position.y < world->size.y && position.x < world->size.x) {
        return &world->matrix[world->size.x * position.y + position.x];
    }

    return nullptr;
}

static inline void set_cell_vec(World *world, VectorU position, Cell cell) {
    world->matrix[world->size.x * position.y + position.x] = cell;
}

static inline Cell *get_cell_ref_i(World *world, uint32_t index) {
    if (index < world->size.x * world->size.y) {
        return &world->matrix[index];
    }

    return nullptr;
}

static inline void set_cell_i(World *world, uint32_t index, Cell cell) { world->matrix[index] = cell; }

static inline void create_wall(World *world, VectorU position, const utf8_char wall_character) {
    set_cell_vec(world, position, create_cell(wall_character, WALL_COLOR, WALL));
}

static inline void create_coin(World *world, VectorU position) {
    set_cell_vec(world, position, create_cell(COIN_CHARACTER, COIN_COLOR, COIN));
}

void free_world(World *world);
