#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(GameoverInterface, 3, 2)
DEFINE_INIT_INTERFACE(GameoverInterface, gameover)

struct GameoverScene {
    Scene base;
    GameoverInterface interface;
};

void init_gameover_scene(GameoverScene *gameover_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameover_scene(void *gameover_scene, void *args);
void update_gameover_scene(void *scene_context);
void draw_gameover_scene(void *gameover_scene);
void exit_gameover_scene(void *gameover_scene, void *);
void free_gameover_scene(GameoverScene *gameover_scene);

static inline void serialize_gameover_scene_transition_args(uint32_t *score, byte *arg_buffer) {
    memcpy(arg_buffer, score, sizeof(uint32_t));
}

static inline void deserialize_gameover_scene_transition_args(uint32_t *score, byte *arg_buffer) {
    memcpy(score, arg_buffer, sizeof(uint32_t));
}
