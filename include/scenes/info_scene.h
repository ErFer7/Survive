#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(InfoInterface, 2, 1)
DEFINE_INIT_INTERFACE(InfoInterface, info)

struct InfoScene {
    Scene base;
    InfoInterface interface;
};

void init_info_scene(InfoScene *info_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_info_scene(void *info_scene, void *);
void update_info_scene(void *scene_context);
void draw_info_scene(void *info_scene);
void exit_info_scene(void *info_scene, void *);
void free_info_scene(InfoScene *info_scene);
