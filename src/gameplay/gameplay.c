#include "gameplay/gameplay.h"

#include "gameplay/cell.h"
#include "gameplay/enemy.h"
#include "gameplay/player.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/random.h"
#include "utils/vector.h"

void init_gameplay(Gameplay *gameplay,
                   VectorU view_size,
                   VectorU parent_size,
                   SceneTransition *gameover_transition,
                   Text *score_info,
                   struct ncplane *scene_plane) {
    init_world(&gameplay->world);

    init_view(&gameplay->view,
              VECTOR_ZERO,
              view_size,
              parent_size,
              DEFAULT_BACKGROUND,
              scene_plane,
              &gameplay->world_mutex);

    init_entity_list(&gameplay->entities);

    pthread_mutex_init(&gameplay->world_mutex, nullptr);

    gameplay->lock_view = false;
    gameplay->player = nullptr;
    gameplay->score = 0;
    gameplay->gameover_transition = malloc(sizeof(SceneTransition));
    gameplay->score_info = score_info;

    memcpy(gameplay->gameover_transition, gameover_transition, sizeof(SceneTransition));
}

void start_gameplay(Gameplay *gameplay, bool restart, VectorU size, bool enable_terrain_generation, bool lock_view) {
    partially_free_gameplay(gameplay);

    gameplay->lock_view = restart ? gameplay->lock_view : lock_view;

    VectorU world_size = restart ? gameplay->world.cell_matrix.size : size;
    bool terrain_generation = restart ? gameplay->world.has_terrain : enable_terrain_generation;

    generate_world(&gameplay->world, world_size, terrain_generation);
    create_player(gameplay, divide_vector_by_scalar(vector_u_to_vector(world_size), 2));

    gameplay->view.position = gameplay->player->position;
    gameplay->score = 0;
}

void create_player(Gameplay *gameplay, Vector position) {
    Entity *entity = allocate_entity(&gameplay->entities);
    gameplay->player = entity;

    init_player_entity(gameplay->player, get_cell_ref_vec(&gameplay->world.cell_matrix, position), position);
}

void create_enemy(Gameplay *gameplay, Vector position) {
    Entity *entity = allocate_entity(&gameplay->entities);

    init_enemy_entity(entity, get_cell_ref_vec(&gameplay->world.cell_matrix, position), position);
}

void update_enemy(Gameplay *gameplay, Entity *enemy) {
    Vector target = gameplay->player->position;
    Vector position = enemy->position;
    bool has_free_path = true;

    int32_t target_direction_x = 0;

    if (target.x > position.x) {
        target_direction_x = 1;
    } else if (target.x < position.x) {
        target_direction_x = -1;
    }

    int32_t target_direction_y = 0;

    if (target.y > position.y) {
        target_direction_y = 1;
    } else if (target.y < position.y) {
        target_direction_y = -1;
    }

    Cell *target_cell = get_cell_ref_vec(&gameplay->world.cell_matrix,
                                         VECTOR(position.x + target_direction_x, position.y + target_direction_y));

    has_free_path = target_cell->type == VOID || target_cell->type == PLAYER;

    if (has_free_path) {
        enemy->direction = VECTOR(target_direction_x, target_direction_y);

        return;
    }

    float distances[8];
    float smallest_distance = INFINITY;
    uint32_t direction_index;
    Vector adjacent_position_vector;

    uint32_t distance_index = 0;

    for (int32_t j = position.y - 1; j < position.y + 2; j++) {
        for (int32_t k = position.x - 1; k < position.x + 2; k++) {
            if (j != position.y || k != position.x) {
                adjacent_position_vector = VECTOR(k, j);
                Cell *cell = get_cell_ref_vec(&gameplay->world.cell_matrix, adjacent_position_vector);

                float penalty = j == enemy->previous_position.y && k == enemy->previous_position.x ? 1.0f : 0.0f;

                if (cell->type == VOID) {
                    distances[distance_index] = vector_distance(adjacent_position_vector, target) + penalty;
                } else {
                    distances[distance_index] = INFINITY;
                }

                distance_index++;
            }
        }
    }

    for (uint32_t j = 0; j < 8; j++) {
        if (smallest_distance > distances[j]) {
            smallest_distance = distances[j];
            direction_index = j;
        }
    }

    enemy->direction = DIRECTIONS[direction_index];
}

void update_entities_movement(Gameplay *gameplay) {
    EntityListIterator iterator;
    EntityListIterator *it = &iterator;

    init_entity_list_iterator(it, &gameplay->entities);

    pthread_mutex_lock(&gameplay->world_mutex);
    while (it != nullptr) {
        Entity *entity = it->entity;

        if (!vector_is_zero(entity->direction)) {
            // This is ok for this game, since there is only two entities
            const float speed = entity->cell->type == PLAYER ? PLAYER_SPEED : ENEMY_SPEED;

            accumulate_movement(entity, speed * entity->speed_modifier);
        }

        if (check_and_reset_movement_accumulator(entity)) {
            Vector new_position = add_vector(entity->position, entity->direction);

            Cell *new_cell = get_cell_ref_vec(&gameplay->world.cell_matrix, new_position);

            if (solve_collision(gameplay, entity, new_cell)) {
                move_entity(entity, new_cell, new_position);
            } else {
                if (gameplay->player == nullptr) {  // Someone killed us!
                    serialize_gameover_scene_transition_args(&gameplay->score, gameplay->gameover_transition->args);

                    transition(gameplay->gameover_transition);
                    break;
                }

                entity->direction = VECTOR_ZERO;
            }
        }

        it = iterate_on_entity_list(it);
    }
    pthread_mutex_unlock(&gameplay->world_mutex);
}

void handle_coin_pick(Gameplay *gameplay) {
    generate_coin(&gameplay->world);
    gameplay->score++;

    char score_info_str[11];

    snprintf(score_info_str, sizeof(score_info_str), "%010d", gameplay->score);

    set_single_line_text_content(gameplay->score_info, score_info_str, sizeof(score_info_str));
}

void generate_enemy(Gameplay *gameplay) {
    VectorU size = gameplay->world.cell_matrix.size;

    while (true) {
        Vector position = VECTOR(inclusive_random(1, size.x - 2), inclusive_random(1, size.y - 2));

        if (vector_distance(gameplay->player->position, position) > MIN_ENEMY_SPAWN_DISTANCE) {
            Cell *cell = get_cell_ref_vec(&gameplay->world.cell_matrix, position);

            if (cell->type == VOID) {
                create_enemy(gameplay, position);
                break;
            }
        }
    }
}

void partially_free_gameplay(Gameplay *gameplay) {
    free_world(&gameplay->world);
    free_entity_list(&gameplay->entities);

    gameplay->player = nullptr;
}

void free_gameplay(Gameplay *gameplay) {
    partially_free_gameplay(gameplay);
    free_view(&gameplay->view);

    if (gameplay->gameover_transition != nullptr) {
        free(gameplay->gameover_transition);
        gameplay->gameover_transition = nullptr;
    }

    pthread_mutex_destroy(&gameplay->world_mutex);
}
