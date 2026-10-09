#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(PauseInterface, 1, 3)
DEFINE_INIT_INTERFACE(PauseInterface, pause)

struct PauseScene {
    Scene base;
    PauseInterface interface;
};

void init_pause_scene(PauseScene *pause_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_pause_scene(void *pause_scene, void *);
void update_pause_scene(void *scene_context);
void draw_pause_scene(void *pause_scene);
void exit_pause_scene(void *pause_scene, void *);
void free_pause_scene(PauseScene *pause_scene);
