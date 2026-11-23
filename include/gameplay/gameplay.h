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
static const float PLAYER_STAMINA_ON_POWERUP_DECREASE_RATE = 2.5f;
static const float PLAYER_STAMINA_ON_POWERUP_INCREASE_RATE = 60.0f;
static const float PLAYER_MAX_STAMINA = 100.0f;
static const float PLAYER_MAX_POWERUP_TIME = 30.0f;  // Seconds

struct Gameplay {
    World world;
    View view;
    EntityList entities;
    pthread_mutex_t world_mutex;
    SceneTransition *gameover_transition;
    Entity *player;
    Text *stamina_info;
    Text *score_info;
    Text *powerup_info;
    uint32_t score;
    float player_stamina;
    float player_powerup_time;
    bool lock_view;
};

void init_gameplay(Gameplay *gameplay,
                   VectorU view_size,
                   VectorU parent_size,
                   SceneTransition *gameover_transition,
                   Text *stamina_info,
                   Text *score_info,
                   Text *powerup_info,
                   struct ncplane *scene_plane);
void start_gameplay(Gameplay *gameplay,
                    bool restart,
                    VectorU size,
                    bool enable_terrain_generation,
                    bool enable_powerup_generation,
                    bool lock_view);
void create_player(Gameplay *gameplay, Vector position);
void create_enemy(Gameplay *gameplay, Vector position);
void update_enemy(Gameplay *gameplay, Entity *enemy, bool flee);
void update_entities_movement(Gameplay *gameplay);
void handle_coin_pick(Gameplay *gameplay);
void handle_powerup_pick(Gameplay *gameplay);
void generate_enemy(Gameplay *gameplay);
void partially_free_gameplay(Gameplay *gameplay);
void free_gameplay(Gameplay *gameplay);

static inline void handle_gameplay_input(Gameplay *gameplay, InputState *input_state) {
    handle_player_input(gameplay->player, input_state, gameplay->player_powerup_time > 0.0f);
}

static inline void update_score_info(Gameplay *gameplay) {
    char score_str[11];

    snprintf(score_str, sizeof(score_str), "%010d", gameplay->score);
    set_single_line_text_content(gameplay->score_info, score_str, sizeof(score_str));
}

static inline void update_stamina_info(Gameplay *gameplay) {
    gameplay->stamina_info->foreground_color = gradient(gameplay->player_stamina / PLAYER_MAX_STAMINA, RED, YELLOW);

    char stamina_str[5];

    snprintf(stamina_str, sizeof(stamina_str), "%03d%%", (int32_t)gameplay->player_stamina);
    set_single_line_text_content(gameplay->stamina_info, stamina_str, sizeof(stamina_str));
}

static inline void update_powerup_info(Gameplay *gameplay) {
    gameplay->powerup_info->foreground_color =
        gradient(gameplay->player_powerup_time / PLAYER_MAX_POWERUP_TIME, VERY_DARK_GRAY, MAGENTA);

    char powerup_str[4];

    snprintf(powerup_str, sizeof(powerup_str), "%02ds", (int32_t)gameplay->player_powerup_time);
    set_single_line_text_content(gameplay->powerup_info, powerup_str, sizeof(powerup_str));
}

static inline void recover_player_stamina(Gameplay *gameplay) {
    if (gameplay->player->speed_modifier < PLAYER_FAST_SPEED_MODIFIER &&
        gameplay->player_stamina < PLAYER_MAX_STAMINA) {
        const float increase_rate = gameplay->player_powerup_time > 0.0f ? PLAYER_STAMINA_ON_POWERUP_INCREASE_RATE
                                                                         : PLAYER_STAMINA_INCREASE_RATE;

        gameplay->player_stamina += (1.0f / (float)UPDATE_FREQUENCY) * increase_rate;

        if (gameplay->player_stamina > PLAYER_MAX_STAMINA) {
            gameplay->player_stamina = PLAYER_MAX_STAMINA;
        }

        update_stamina_info(gameplay);
    }
}

static inline void consume_player_stamina(Gameplay *gameplay) {
    if (gameplay->player->speed_modifier > PLAYER_DEFAULT_SPEED_MODIFIER) {
        const float deacrease_rate = gameplay->player_powerup_time > 0.0f ? PLAYER_STAMINA_ON_POWERUP_DECREASE_RATE
                                                                          : PLAYER_STAMINA_DECREASE_RATE;

        gameplay->player_stamina -= (1.0f / (float)UPDATE_FREQUENCY) * deacrease_rate;

        if (gameplay->player_stamina < 0.0f) {
            gameplay->player_stamina = 0.0f;
            gameplay->player->speed_modifier = PLAYER_DEFAULT_SPEED_MODIFIER;
        }

        update_stamina_info(gameplay);
    }
}

static inline void update_player_powerup(Gameplay *gameplay) {
    if (gameplay->player_powerup_time > 0.0f) {
        gameplay->player_powerup_time -= (1.0f / (float)UPDATE_FREQUENCY);

        if (gameplay->player_powerup_time < 0.0f) {
            gameplay->player_powerup_time = 0.0f;
            generate_powerup(&gameplay->world);
        }

        update_powerup_info(gameplay);
    }
}

static inline void update_enemies(Gameplay *gameplay) {
    EntityListIterator iterator;
    EntityListIterator *it = &iterator;

    init_entity_list_iterator(it, &gameplay->entities);

    // The enemies are allocated after the player, that's why there is this initial iteration here
    it = iterate_on_entity_list(it);

    while (it != nullptr) {
        update_enemy(gameplay, it->entity, gameplay->player_powerup_time > 0.0f);
        it = iterate_on_entity_list(it);
    }
}

static inline void update_gameplay(Gameplay *gameplay, InputState *input_state) {
    handle_gameplay_input(gameplay, input_state);
    update_player_powerup(gameplay);
    update_enemies(gameplay);
    update_entities_movement(gameplay);

    if (gameplay->player != nullptr && !gameplay->lock_view) {
        gameplay->view.position = gameplay->player->position;
    }
}

// TODO: Uninline this
static inline bool solve_collision(Gameplay *gameplay, Entity *entity, Cell *cell, Entity **killed_entity) {
    if (entity->cell->type == PLAYER) {
        switch (cell->type) {
            case COIN:
                handle_coin_pick(gameplay);
                return true;
            case POWERUP:
                handle_powerup_pick(gameplay);
                return true;
            case VOID:
                return true;
            case ENEMY:
                if (gameplay->player_powerup_time > 0.0f) {
                    *killed_entity = cell->entity_ptr;
                    deactivate_entity(*killed_entity);  // Only needed for enemies
                    return true;
                }

                *killed_entity = entity;
                return false;
            default:
                return false;
        }
    } else if (entity->cell->type == ENEMY) {
        switch (cell->type) {
            case VOID:
                return true;
            case PLAYER:
                if (gameplay->player_powerup_time > 0.0f) {
                    *killed_entity = entity;
                    deactivate_entity(*killed_entity);  // Only needed for enemies
                } else {
                    *killed_entity = cell->entity_ptr;
                }

                return false;
            default:
                return false;
        }
    }

    return false;
}

static inline void draw_gameplay(Gameplay *gameplay) { draw_world_on_view(&gameplay->view, &gameplay->world); }

static inline void resize_gameplay_view(Gameplay *gameplay, VectorU size) { resize_view(&gameplay->view, size); }
