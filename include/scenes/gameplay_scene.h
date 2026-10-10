#pragma once

#include "gameplay/gameplay.h"
#include "interface/interface.h"
#include "scene.h"
#include "types.h"

DEFINE_BUTTONLESS_INTERFACE(GameplayInterface, 6)
DEFINE_INIT_BUTTONLESS_INTERFACE(GameplayInterface, gameplay)

enum GameplayTransitionMode {
    RESET = 0b001,
    PRESERVE = 0b010,

    GENERATE = 0b100,

    START = GENERATE | RESET,
    RESTART = GENERATE | PRESERVE,

    IGNORE = 0b1000,
    RESUME = IGNORE,
    PAUSE = IGNORE,
};

struct GameplayScene {
    Scene base;
    GameplayInterface interface;
    Gameplay gameplay;
};

struct GameplaySceneArgs {
    enum GameplayTransitionMode transition_mode;
    bool enable_terrain_generation;
    VectorU world_size;
};

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameplay_scene(void *gameplay_scene, void *args);
void update_gameplay_scene(void *scene_context);
void draw_gameplay_scene(void *gameplay_scene);
void exit_gameplay_scene(void *gameplay_scene, void *args);
void free_gameplay_scene(GameplayScene *gameplay_scene);

static inline GameplaySceneArgs create_gameplay_scene_args(enum GameplayTransitionMode transition_mode,
                                                           bool enable_terrain_generation,
                                                           VectorU world_size) {
    GameplaySceneArgs gameplay_scene_args = {transition_mode, enable_terrain_generation, world_size};

    return gameplay_scene_args;
}

static inline void serialize_gameplay_scene_transition_args(GameplaySceneArgs *gameplay_scene_args, byte *arg_buffer) {
    memcpy(arg_buffer, gameplay_scene_args, sizeof(GameplaySceneArgs));
}

static inline void deserialize_gameplay_scene_transition_args(GameplaySceneArgs *gameplay_scene_args,
                                                              byte *arg_buffer) {
    memcpy(gameplay_scene_args, arg_buffer, sizeof(GameplaySceneArgs));
}
