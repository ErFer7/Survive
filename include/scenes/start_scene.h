#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(StartInterface, 3, 6)
DEFINE_INIT_INTERFACE(StartInterface, start)

struct StartScene {
    Scene base;
    StartInterface interface;
};

void init_start_scene(StartScene *start_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_start_scene(void *start_scene, void *);
void update_start_scene(void *scene_context);
void draw_start_scene(void *start_scene);
void exit_start_scene(void *start_scene, void *);
void free_start_scene(StartScene *start_scene);
