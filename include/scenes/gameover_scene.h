#pragma once

#include "interface/interface.h"
#include "scene.h"
#include "scenes/scene_transition.h"

DEFINE_INTERFACE(GameoverInterface, 3, 2)
DEFINE_INIT_INTERFACE(GameoverInterface, gameover)

struct GameoverScene {
    Scene base;
    GameoverInterface interface;
    SceneTransition restart_transition;
    SceneTransition menu_transition;
};

void init_gameover_scene(GameoverScene *gameover_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameover_scene(void *gameover_scene, void *args);
void update_gameover_scene(void *scene_context);
void draw_gameover_scene(void *gameover_scene);
void exit_gameover_scene(void *gameover_scene, void *);
int gameover_resize_callback(struct ncplane *parent_plane);
void free_gameover_scene(GameoverScene *gameover_scene);
