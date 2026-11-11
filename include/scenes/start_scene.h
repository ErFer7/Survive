#pragma once

#include "interface/interface.h"
#include "scene.h"
#include "scenes/scene_transition.h"

DEFINE_INTERFACE(StartInterface, 3, 6)
DEFINE_INIT_INTERFACE(StartInterface, start)

struct StartScene {
    Scene base;
    StartInterface interface;
    SceneTransition menu_transition;
    SceneTransition gameplay_transition_regular;
    SceneTransition gameplay_transition_small;
    SceneTransition gameplay_transition_large;
    SceneTransition gameplay_transition_mega;
    SceneTransition gameplay_transition_classic;
};

void init_start_scene(StartScene *start_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_start_scene(void *start_scene, void *);
void update_start_scene(void *scene_context);
void draw_start_scene(void *start_scene);
void exit_start_scene(void *start_scene, void *);
int start_resize_callback(struct ncplane *parent_plane);
void free_start_scene(StartScene *start_scene);
