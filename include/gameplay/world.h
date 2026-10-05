#pragma once

#include "cell.h"
#include "gameplay/entity.h"
#include "gameplay/player.h"
#include "utils/vector.h"

static const Color BACKGROUND_COLOR = HYPER_DARK_GRAY;
static const Color WALL_COLOR = WHITE;

static const char BACKGROUND_CHARACTER = ' ';
static const char WALL_CHARACTER = '#';

struct World {
    Cell *matrix;
    Vector size;
    Entity player;
    Cell **coins;
    Entity *enemies;
};

void init_world(World *world, Vector size);

static inline Cell *get_cell_ref_xy(World *world, int row, int column) {
    return &world->matrix[world->size.x * row + column];
}

static inline void set_cell_xy(World *world, int row, int column, Cell cell) {
    world->matrix[world->size.x * row + column] = cell;
}

static inline Cell *get_cell_ref_vec(World *world, Vector position) {
    return &world->matrix[world->size.x * position.y + position.x];
}

static inline void set_cell_vec(World *world, Vector position, Cell cell) {
    world->matrix[world->size.x * position.y + position.x] = cell;
}

static inline Cell *get_cell_ref_i(World *world, unsigned int index) { return &world->matrix[index]; }

static inline void set_cell_i(World *world, unsigned int index, Cell cell) { world->matrix[index] = cell; }

static inline void create_player(World *world, Vector position) {
    world->player = create_player_entity(get_cell_ref_vec(world, position), position);
}

static inline void create_wall(World *world, Vector position) {
    set_cell_vec(world, position, create_cell(WALL_CHARACTER, WALL_COLOR));
}

static inline void handle_world_input(World *world, int key_a, int key_b) {
    handle_player_input(&world->player, key_a, key_b);
}

void update_entities(World *world);

void free_world(World *world);
