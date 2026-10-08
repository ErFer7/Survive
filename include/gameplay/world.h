#pragma once

#include "cell.h"
#include "gameplay/entity.h"
#include "gameplay/player.h"
#include "utils/vector.h"

static const Color BACKGROUND_COLOR = HYPER_DARK_GRAY;

static const char BACKGROUND_CHARACTER = ' ';

static const utf8_char OPAQUE_WALL_CHARACTER = UTF8("█");
static const utf8_char DARK_WALL_CHARACTER = UTF8("▓");
static const utf8_char MEDIUM_WALL_CHARACTER = UTF8("▒");
static const utf8_char FAINT_WALL_CHARACTER = UTF8("░");
static const Color WALL_COLOR = WHITE;

struct World {
    Cell *matrix;
    Vector size;
    bool has_terrain;
};

void init_world(World *world);
void generate_world(World *world, Vector size, bool enable_terrain_generation);
void generate_walls(World *world);
void generate_terrain(World *world);

static inline Cell *get_cell_ref_xy(World *world, int row, int column) {
    if (row >= 0 && row < world->size.y && column >= 0 && column < world->size.x) {
        return &world->matrix[world->size.x * row + column];
    }

    return nullptr;
}

static inline void set_cell_xy(World *world, int row, int column, Cell cell) {
    world->matrix[world->size.x * row + column] = cell;
}

static inline Cell *get_cell_ref_vec(World *world, Vector position) {
    if (position.y >= 0 && position.y < world->size.y && position.x >= 0 && position.x < world->size.x) {
        return &world->matrix[world->size.x * position.y + position.x];
    }

    return nullptr;
}

static inline void set_cell_vec(World *world, Vector position, Cell cell) {
    world->matrix[world->size.x * position.y + position.x] = cell;
}

static inline Cell *get_cell_ref_i(World *world, unsigned int index) {
    if (index < world->size.x * world->size.y) {
        return &world->matrix[index];
    }

    return nullptr;
}

static inline void set_cell_i(World *world, unsigned int index, Cell cell) { world->matrix[index] = cell; }

static inline void create_wall(World *world, Vector position, const utf8_char wall_character) {
    set_cell_vec(world, position, create_cell(wall_character, WALL_COLOR, WALL));
}

void free_world(World *world);
