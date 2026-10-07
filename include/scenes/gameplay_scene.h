#pragma once

#include "gameplay/view.h"
#include "gameplay/world.h"
#include "interface/interface.h"
#include "scene.h"
#include "types.h"

DEFINE_BUTTONLESS_INTERFACE(GameplayInterface, 6);
DEFINE_INIT_BUTTONLESS_INTERFACE(GameplayInterface, gameplay);

static const char *FPS_LABEL = "FPS:";

static const char *TICKS_LABEL = "TPS:";

static const char *SCORE_LABEL = "Score:";

struct GameplayScene {
    Scene base;
    GameplayInterface interface;
    World world;
    View view;
};

struct GameplaySceneArgs {
    bool reset;
    bool enable_terrain_generation;
    uint16_t world_width;
    uint16_t world_height;
};

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameplay_scene(void *gameplay_scene, void *args);
void update_gameplay_scene(void *scene_context);
void draw_gameplay_scene(void *gameplay_scene);
void exit_gameplay_scene(void *gameplay_scene);
void free_gameplay_scene(GameplayScene *gameplay_scene);

static inline GameplaySceneArgs create_gameplay_scene_args(bool reset,
                                                           uint16_t world_width,
                                                           uint16_t world_height,
                                                           bool enable_terrain_generation) {
    GameplaySceneArgs gameplay_scene_args = {reset, enable_terrain_generation, world_height, world_height};

    return gameplay_scene_args;
}

static inline void serialize_gameplay_scene_transition_args(GameplaySceneArgs *gameplay_scene_args, byte *arg_buffer) {
    memcpy(arg_buffer, gameplay_scene_args, sizeof(GameplaySceneArgs));
}

static inline void deserialize_gameplay_scene_transition_args(GameplaySceneArgs *gameplay_scene_args,
                                                              byte *arg_buffer) {
    memcpy(gameplay_scene_args, arg_buffer, sizeof(GameplaySceneArgs));
}
