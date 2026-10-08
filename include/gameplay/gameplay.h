#pragma once

#include <notcurses/notcurses.h>

#include "gameplay/view.h"
#include "gameplay/world.h"
#include "types.h"

struct Gameplay {
    World world;
    View view;
    Entity *entities;
    Entity *player;
    Entity *enemies;
    uint32_t entity_count;
    uint16_t score;
};

void init_gameplay(Gameplay *gameplay, Vector view_size, Vector parent_size, struct ncplane *scene_plane);
void start_gameplay(Gameplay *gameplay, bool restart, Vector size, bool enable_terrain_generation);

Entity *add_entity(Gameplay *gameplay, const Entity entity);

static inline void create_player(Gameplay *gameplay, Vector position) {
    gameplay->player =
        add_entity(gameplay, create_player_entity(get_cell_ref_vec(&gameplay->world, position), position));
}

static inline void handle_gameplay_input(Gameplay *gameplay, InputState *input_state) {
    handle_player_input(gameplay->player, input_state);
}

void update_entities_movement(Gameplay *gameplay);

static inline void update_gameplay(Gameplay *gameplay, InputState *input_state) {
    handle_gameplay_input(gameplay, input_state);
    update_entities_movement(gameplay);
    update_view_position(&gameplay->view, gameplay->player->position);
}

// TODO: Check if this is decent enough
static inline bool solve_collision(Entity *entity, Cell *cell) {
    if (entity->cell->type == PLAYER) {
        switch (cell->type) {
            case COIN:
                // TODO: Handle coin
            case VOID:
                return true;
            case ENEMY:
                // TODO: Handle enemy
            default:
                return false;
        }
    } else if (entity->cell->type == ENEMY) {
        switch (cell->type) {
            case VOID:
                return true;
            case PLAYER:
                // TODO: Handle player
            default:
                return false;
        }
    }

    return false;
}

static inline void draw_gameplay(Gameplay *gameplay) { draw_world_on_view(&gameplay->view, &gameplay->world); }

void end_gameplay(Gameplay *gameplay);

void free_gameplay(Gameplay *gameplay);
