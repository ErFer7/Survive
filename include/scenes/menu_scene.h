#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(MenuInterface, 2, 3)
DEFINE_INIT_INTERFACE(MenuInterface, menu)

struct MenuScene {
    Scene base;
    MenuInterface interface;
};

void init_menu_scene(MenuScene *menu_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_menu_scene(void *menu_scene, void *);
void update_menu_scene(void *scene_context);
void draw_menu_scene(void *menu_scene);
void exit_menu_scene(void *menu_scene, void *);
void free_menu_scene(MenuScene *menu_scene);
