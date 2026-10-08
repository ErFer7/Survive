#include "gameplay/gameplay.h"

#include "gameplay/cell.h"

void init_gameplay(Gameplay *gameplay, Vector view_size, Vector parent_size, struct ncplane *scene_plane) {
    init_world(&gameplay->world);

    init_view(&gameplay->view, VECTOR_ZERO, view_size, parent_size, scene_plane);

    gameplay->entities = nullptr;
    gameplay->entity_count = 0;
    gameplay->player = nullptr;
    gameplay->enemies = nullptr;
    gameplay->score = 0;
}

void start_gameplay(Gameplay *gameplay, bool restart, Vector size, bool enable_terrain_generation) {
    free_world(&gameplay->world);

    Vector world_size = restart ? gameplay->world.size : size;
    bool terrain_generation = restart ? gameplay->world.has_terrain : enable_terrain_generation;

    generate_world(&gameplay->world, world_size, terrain_generation);

    create_player(gameplay, create_vector(world_size.x / 2, world_size.y / 2));

    update_view_position(&gameplay->view, gameplay->player->position);

    gameplay->score = 0;
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

            if (solve_collision(&entities[i], new_cell)) {
                move_entity(&entities[i], new_cell, new_position);
            } else {
                entities[i].direction = VECTOR_ZERO;
            }
        }
    }
}

void end_gameplay(Gameplay *gameplay) {
    free_world(&gameplay->world);

    if (gameplay->entities != nullptr) {
        free(gameplay->entities);
    }

    gameplay->player = nullptr;
    gameplay->enemies = nullptr;
}

void free_gameplay(Gameplay *gameplay) {
    end_gameplay(gameplay);
    free_view(&gameplay->view);
}
