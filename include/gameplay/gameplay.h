#pragma once

#include <notcurses/notcurses.h>

#include "gameplay/enemy.h"
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
    uint32_t enemy_count;
    uint16_t score;
    SceneTransition *gameover_transition;
};

void init_gameplay(Gameplay *gameplay,
                   Vector view_size,
                   Vector parent_size,
                   SceneTransition *gameover_transition,
                   struct ncplane *scene_plane);
void start_gameplay(Gameplay *gameplay, bool restart, Vector size, bool enable_terrain_generation);

void allocate_entity(Gameplay *gameplay);

void create_player(Gameplay *gameplay, Vector position);
void create_enemy(Gameplay *gameplay, Vector position);

static inline void handle_gameplay_input(Gameplay *gameplay, InputState *input_state) {
    handle_player_input(gameplay->player, input_state);
}

void update_enemy(Gameplay *gameplay, Entity *enemy);
void update_entities_movement(Gameplay *gameplay);

static inline void update_enemies(Gameplay *gameplay) {
    for (uint32_t i = 0; i < gameplay->enemy_count; i++) {
        update_enemy(gameplay, &gameplay->enemies[i]);
    }
}

static inline void update_gameplay(Gameplay *gameplay, InputState *input_state) {
    handle_gameplay_input(gameplay, input_state);
    update_enemies(gameplay);
    update_entities_movement(gameplay);

    if (gameplay->player != nullptr) {
        update_view_position(&gameplay->view, gameplay->player->position);
    }
}

void handle_coin_pick(Gameplay *gameplay);

void generate_enemy(Gameplay *gameplay);

// TODO: Check if this is decent enough
static inline bool solve_collision(Gameplay *gameplay, Entity *entity, Cell *cell) {
    if (entity->cell->type == PLAYER) {
        switch (cell->type) {
            case COIN:
                handle_coin_pick(gameplay);
                generate_enemy(gameplay);
            case VOID:
                return true;
            case ENEMY:
                gameplay->player = nullptr;
            default:
                return false;
        }
    } else if (entity->cell->type == ENEMY) {
        switch (cell->type) {
            case VOID:
                return true;
            case PLAYER:
                gameplay->player = nullptr;
            default:
                return false;
        }
    }

    return false;
}

static inline void draw_gameplay(Gameplay *gameplay) { draw_world_on_view(&gameplay->view, &gameplay->world); }

void partially_free_gameplay(Gameplay *gameplay);

void free_gameplay(Gameplay *gameplay);
