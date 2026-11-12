#pragma once

#include <notcurses/notcurses.h>
#include <pthread.h>

#include "gameplay/entity_list.h"
#include "gameplay/player.h"
#include "gameplay/view.h"
#include "gameplay/world.h"
#include "interface/text.h"
#include "types.h"

static const float MIN_ENEMY_SPAWN_DISTANCE = 20.0f;
static const float PLAYER_STAMINA_DECREASE_RATE = 10.0f;
static const float PLAYER_STAMINA_INCREASE_RATE = 15.0f;

struct Gameplay {
    World world;
    View view;
    EntityList entities;
    pthread_mutex_t world_mutex;
    SceneTransition *gameover_transition;
    Entity *player;
    Text *stamina_info;
    Text *score_info;
    uint32_t score;
    float player_stamina;
    bool lock_view;
};

void init_gameplay(Gameplay *gameplay,
                   VectorU view_size,
                   VectorU parent_size,
                   SceneTransition *gameover_transition,
                   Text *stamina_info,
                   Text *score_info,
                   struct ncplane *scene_plane);
void start_gameplay(Gameplay *gameplay, bool restart, VectorU size, bool enable_terrain_generation, bool lock_view);
void create_player(Gameplay *gameplay, Vector position);
void create_enemy(Gameplay *gameplay, Vector position);
void update_enemy(Gameplay *gameplay, Entity *enemy);
void update_entities_movement(Gameplay *gameplay);
void handle_coin_pick(Gameplay *gameplay);
void generate_enemy(Gameplay *gameplay);
void partially_free_gameplay(Gameplay *gameplay);
void free_gameplay(Gameplay *gameplay);

static inline void handle_gameplay_input(Gameplay *gameplay, InputState *input_state) {
    handle_player_input(gameplay->player, input_state);
}

static inline void update_enemies(Gameplay *gameplay) {
    EntityListIterator iterator;
    EntityListIterator *it = &iterator;

    init_entity_list_iterator(it, &gameplay->entities);

    // The enemies are allocated after the player, that's why there is this initial iteration here
    it = iterate_on_entity_list(it);

    while (it != nullptr) {
        update_enemy(gameplay, it->entity);
        it = iterate_on_entity_list(it);
    }
}

static inline void update_gameplay(Gameplay *gameplay, InputState *input_state) {
    handle_gameplay_input(gameplay, input_state);
    update_enemies(gameplay);
    update_entities_movement(gameplay);

    if (gameplay->player != nullptr && !gameplay->lock_view) {
        gameplay->view.position = gameplay->player->position;
    }
}

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

static inline void resize_gameplay_view(Gameplay *gameplay, VectorU size) { resize_view(&gameplay->view, size); }

static inline void update_score_info(Gameplay *gameplay) {
    char score_str[11];

    snprintf(score_str, sizeof(score_str), "%010d", gameplay->score);
    set_single_line_text_content(gameplay->score_info, score_str, sizeof(score_str));
}

static inline void update_stamina_info(Gameplay *gameplay) {
    gameplay->stamina_info->foreground_color = gradient(gameplay->player_stamina / 100.0f, RED, YELLOW);

    char stamina_str[5];

    snprintf(stamina_str, sizeof(stamina_str), "%03d%%", (int32_t)gameplay->player_stamina);
    set_single_line_text_content(gameplay->stamina_info, stamina_str, sizeof(stamina_str));
}
