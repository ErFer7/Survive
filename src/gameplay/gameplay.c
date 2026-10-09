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

    init_view(&gameplay->view, VECTORU_ZERO, view_size, parent_size, HYPER_DARK_GRAY, scene_plane);

    gameplay->entities = nullptr;
    gameplay->entity_count = 0;
    gameplay->player = nullptr;
    gameplay->enemies = nullptr;
    gameplay->score = 0;
    gameplay->gameover_transition = malloc(sizeof(SceneTransition));
    gameplay->score_info = score_info;

    memcpy(gameplay->gameover_transition, gameover_transition, sizeof(SceneTransition));
}

void start_gameplay(Gameplay *gameplay, bool restart, VectorU size, bool enable_terrain_generation) {
    partially_free_gameplay(gameplay);

    VectorU world_size = restart ? gameplay->world.size : size;
    bool terrain_generation = restart ? gameplay->world.has_terrain : enable_terrain_generation;

    generate_world(&gameplay->world, world_size, terrain_generation);
    create_player(gameplay, divide_vector_u_by_scalar(world_size, 2));
    update_view_position(&gameplay->view, gameplay->player->position);

    gameplay->score = 0;
}

void allocate_entity(Gameplay *gameplay) {
    gameplay->entity_count++;

    if (gameplay->entities == nullptr) {
        gameplay->entities = malloc(sizeof(Entity));
    } else {
        gameplay->entities = realloc(gameplay->entities, sizeof(Entity) * gameplay->entity_count);
        gameplay->enemies = &gameplay->entities[1];
    }

    gameplay->player = &gameplay->entities[0];
}

Entity *add_entity(Gameplay *gameplay, const Entity entity) {
    gameplay->entity_count++;

    if (gameplay->entities == nullptr) {
        gameplay->entities = malloc(sizeof(Entity));
    } else {
        gameplay->entities = realloc(gameplay->entities, sizeof(Entity) * gameplay->entity_count);
    }

    uint32_t last = gameplay->entity_count - 1;

    gameplay->entities[last] = entity;

    return &gameplay->entities[last];
}

void create_player(Gameplay *gameplay, VectorU position) {
    allocate_entity(gameplay);

    *gameplay->player = create_player_entity(get_cell_ref_vec(&gameplay->world, position), position);
}

void create_enemy(Gameplay *gameplay, VectorU position) {
    allocate_entity(gameplay);

    gameplay->enemies[gameplay->enemy_count] =
        create_enemy_entity(get_cell_ref_vec(&gameplay->world, position), position);

    gameplay->enemy_count++;
}

// PERFORMANCE: Optimize
void update_enemy(Gameplay *gameplay, Entity *enemy) {
    VectorU target = gameplay->player->position;
    VectorU position = enemy->position;
    bool blocked = false;

    int32_t target_direction_x = 0;

    if (target.x > position.x) {
        target_direction_x = 1;
    } else if (target.x < position.x) {
        target_direction_x = -1;
    }

    Cell *target_cell =
        get_cell_ref_vec(&gameplay->world, create_vector_u(position.x + target_direction_x, position.y));

    blocked = target_cell->type != VOID && target_cell->type != PLAYER;

    if (!blocked) {
        enemy->direction.x = target_direction_x;
    }

    int32_t target_direction_y = 0;

    if (target.y > position.y) {
        target_direction_y = 1;
    } else if (target.y < position.y) {
        target_direction_y = -1;
    }

    target_cell = get_cell_ref_vec(&gameplay->world, create_vector_u(position.x, position.y + target_direction_y));

    blocked = target_cell->type != VOID && target_cell->type != PLAYER;

    if (!blocked) {
        enemy->direction.y = target_direction_y;
    }

    if (blocked) {
        float distances[8];
        float smallest_distance = INFINITY;
        uint32_t direction_index;
        VectorU adjacent_position_vector;

        uint32_t distance_index = 0;

        for (uint32_t j = position.x - 1; j < position.x + 2; j++) {
            for (uint32_t k = position.y - 1; k < position.y + 2; k++) {
                if (j != position.x || k != position.y) {
                    adjacent_position_vector = create_vector_u(j, k);
                    Cell *cell = get_cell_ref_vec(&gameplay->world, adjacent_position_vector);

                    if (cell->type == VOID) {
                        distances[distance_index] = vector_u_distance(adjacent_position_vector, target);
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

        switch (direction_index) {
            case 0:
                enemy->direction.x = -1;
                enemy->direction.y = -1;
                break;
            case 1:
                enemy->direction.x = -1;
                enemy->direction.y = 0;
                break;
            case 2:
                enemy->direction.x = -1;
                enemy->direction.y = 1;
                break;
            case 3:
                enemy->direction.x = 0;
                enemy->direction.y = -1;
                break;
            case 4:
                enemy->direction.x = 0;
                enemy->direction.y = 1;
                break;
            case 5:
                enemy->direction.x = 1;
                enemy->direction.y = -1;
                break;
            case 6:
                enemy->direction.x = 1;
                enemy->direction.y = 0;
                break;
            case 7:
                enemy->direction.x = 1;
                enemy->direction.y = 1;
                break;
            default:
                break;
        }
    }
}

void update_entities_movement(Gameplay *gameplay) {
    Entity *entities = gameplay->entities;

    for (uint32_t i = 0; i < gameplay->entity_count; i++) {
        if (!vector_is_zero(entities[i].direction)) {
            // NOTE: This is ok for this game, since there is only two entities
            const float speed = entities[i].cell->type == PLAYER ? PLAYER_SPEED : ENEMY_SPEED;

            accumulate_movement(&entities[i], speed * entities[i].speed_modifier);
        }

        if (check_and_reset_movement_accumulator(&entities[i])) {
            VectorU new_position =
                vector_to_vector_u(add_vector(vector_u_to_vector(entities[i].position), entities[i].direction));

            Cell *new_cell = get_cell_ref_vec(&gameplay->world, new_position);

            if (solve_collision(gameplay, &entities[i], new_cell)) {
                move_entity(&entities[i], new_cell, new_position);
            } else {
                if (gameplay->player == nullptr) {  // Someone killed us!
                    serialize_gameover_scene_transition_args(&gameplay->score, gameplay->gameover_transition->args);

                    transition(gameplay->gameover_transition);
                    break;
                }

                entities[i].direction = VECTOR_ZERO;
            }
        }
    }
}

void handle_coin_pick(Gameplay *gameplay) {
    generate_coin(&gameplay->world);
    gameplay->score++;

    char score_info_str[11];

    snprintf(score_info_str, sizeof(score_info_str), "%010d", gameplay->score);

    set_single_line_text_content(gameplay->score_info, score_info_str, sizeof(score_info_str));
}

void generate_enemy(Gameplay *gameplay) {
    while (true) {
        VectorU position = create_vector_u(inclusive_random(1, gameplay->world.size.x - 2),
                                           inclusive_random(1, gameplay->world.size.y - 2));

        if (vector_u_distance(gameplay->player->position, position) > 20.0f) {
            Cell *cell = get_cell_ref_vec(&gameplay->world, position);

            if (cell->type == VOID) {
                create_enemy(gameplay, position);
                break;
            }
        }
    }
}

void partially_free_gameplay(Gameplay *gameplay) {
    free_world(&gameplay->world);

    if (gameplay->entities != nullptr) {
        free(gameplay->entities);
        gameplay->entities = nullptr;
        gameplay->entity_count = 0;
    }

    gameplay->player = nullptr;
    gameplay->enemies = nullptr;
    gameplay->enemy_count = 0;
}

void free_gameplay(Gameplay *gameplay) {
    partially_free_gameplay(gameplay);
    free_view(&gameplay->view);

    if (gameplay->gameover_transition != nullptr) {
        free(gameplay->gameover_transition);
        gameplay->gameover_transition = nullptr;
    }
}
