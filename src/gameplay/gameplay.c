#include "gameplay/gameplay.h"

#include "gameplay/cell.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/random.h"
#include "utils/vector.h"

void init_gameplay(Gameplay *gameplay,
                   Vector view_size,
                   Vector parent_size,
                   SceneTransition *gameover_transition,
                   struct ncplane *scene_plane) {
    init_world(&gameplay->world);

    init_view(&gameplay->view, VECTOR_ZERO, view_size, parent_size, scene_plane);

    gameplay->entities = nullptr;
    gameplay->entity_count = 0;
    gameplay->player = nullptr;
    gameplay->enemies = nullptr;
    gameplay->score = 0;
    gameplay->gameover_transition = malloc(sizeof(SceneTransition));

    memcpy(gameplay->gameover_transition, gameover_transition, sizeof(SceneTransition));
}

void start_gameplay(Gameplay *gameplay, bool restart, Vector size, bool enable_terrain_generation) {
    partially_free_gameplay(gameplay);

    Vector world_size = restart ? gameplay->world.size : size;
    bool terrain_generation = restart ? gameplay->world.has_terrain : enable_terrain_generation;

    generate_world(&gameplay->world, world_size, terrain_generation);
    create_player(gameplay, create_vector(world_size.x / 2, world_size.y / 2));
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

void create_player(Gameplay *gameplay, Vector position) {
    allocate_entity(gameplay);

    *gameplay->player = create_player_entity(get_cell_ref_vec(&gameplay->world, position), position);
}

void create_enemy(Gameplay *gameplay, Vector position) {
    allocate_entity(gameplay);

    gameplay->enemies[gameplay->enemy_count] =
        create_enemy_entity(get_cell_ref_vec(&gameplay->world, position), position);

    gameplay->enemy_count++;
}

// PERFORMANCE: Optimize
void update_enemy(Gameplay *gameplay, Entity *enemy) {
    Vector target = gameplay->player->position;
    Vector position = enemy->position;
    bool blocked = false;

    int32_t target_direction_x = 0;

    if (target.x > position.x) {
        target_direction_x = 1;
    } else if (target.x < position.x) {
        target_direction_x = -1;
    }

    Cell *target_cell = get_cell_ref_vec(&gameplay->world, create_vector(position.x + target_direction_x, position.y));

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

    target_cell = get_cell_ref_vec(&gameplay->world, create_vector(position.x, position.y + target_direction_y));

    blocked = target_cell->type != VOID && target_cell->type != PLAYER;

    if (!blocked) {
        enemy->direction.y = target_direction_y;
    }

    if (blocked) {
        float distances[8];
        float smallest_distance = INFINITY;
        int direction_index;
        Vector adjacent_position_vector;

        int distance_index = 0;

        for (int j = position.x - 1; j < position.x + 2; j++) {
            for (int k = position.y - 1; k < position.y + 2; k++) {
                if (j != position.x || k != position.y) {
                    adjacent_position_vector = create_vector(j, k);
                    Cell *cell = get_cell_ref_vec(&gameplay->world, adjacent_position_vector);

                    if (cell->type == VOID) {
                        distances[distance_index] =
                            distance(vector_to_vector_f(adjacent_position_vector), vector_to_vector_f(target));
                    } else {
                        distances[distance_index] = INFINITY;
                    }

                    distance_index++;
                }
            }
        }

        for (int j = 0; j < 8; j++) {
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
        if (is_non_zero(entities[i].direction)) {
            // NOTE: This is ok for this game, since there is only two entities
            const float speed = entities[i].cell->type == PLAYER ? PLAYER_SPEED : ENEMY_SPEED;

            accumulate_movement(&entities[i], speed);
        }

        if (check_and_reset_movement_accumulator(&entities[i])) {
            Vector new_position = add_vector(entities[i].position, entities[i].direction);
            Cell *new_cell = get_cell_ref_vec(&gameplay->world, new_position);

            if (solve_collision(gameplay, &entities[i], new_cell)) {
                move_entity(&entities[i], new_cell, new_position);
            } else {
                if (gameplay->player == nullptr) {  // Someone killed us!
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
}

void generate_enemy(Gameplay *gameplay) {
    while (true) {
        Vector position = create_vector(inclusive_random(1, gameplay->world.size.x - 2),
                                        inclusive_random(1, gameplay->world.size.y - 2));

        VectorF player_position = vector_to_vector_f(gameplay->player->position);
        VectorF position_f = vector_to_vector_f(position);

        if (distance(player_position, position_f) > 20.0f) {
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
