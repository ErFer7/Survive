#pragma once

#include "gameplay/gameplay.h"
#include "interface/interface.h"
#include "scene.h"
#include "scene_transition.h"
#include "types.h"

DEFINE_BUTTONLESS_INTERFACE(GameplayInterface, 8)
DEFINE_INIT_BUTTONLESS_INTERFACE(GameplayInterface, gameplay)

enum GameplayTransitionMode {
    RESET = 0b001,
    PRESERVE = 0b010,

    GENERATE = 0b100,

    START = GENERATE | RESET,
    RESTART = GENERATE | PRESERVE,

    IGNORE = 0b1000,
    FREE = 0b10000,
    RESUME = IGNORE,
    PAUSE = IGNORE,
    GAMEOVER = FREE,
};

struct GameplayScene {
    Scene base;
    GameplayInterface interface;
    SceneTransition pause_transition;
    SceneTransition gameover_transition;
    Gameplay gameplay;
};

struct GameplaySceneEnterArgs {
    enum GameplayTransitionMode transition_mode;
    bool enable_terrain_generation;
    bool lock_view;
    VectorU world_size;
};

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameplay_scene(void *gameplay_scene, void *args);
void update_gameplay_scene(void *scene_context);
void draw_gameplay_scene(void *gameplay_scene);
void exit_gameplay_scene(void *gameplay_scene, void *args);
int gameplay_resize_callback(struct ncplane *parent_plane);
void pause_transition_wrapper(void *gameplay_scene);
void free_gameplay_scene(GameplayScene *gameplay_scene);

static inline void init_gameplay_scene_args(GameplaySceneEnterArgs *gameplay_scene_args,
                                            enum GameplayTransitionMode transition_mode,
                                            bool lock_view,
                                            bool enable_terrain_generation,
                                            VectorU world_size) {
    gameplay_scene_args->transition_mode = transition_mode,
    gameplay_scene_args->enable_terrain_generation = enable_terrain_generation;
    gameplay_scene_args->lock_view = lock_view;
    gameplay_scene_args->world_size = world_size;
}
