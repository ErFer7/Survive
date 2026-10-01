#pragma once

#include "../interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(MenuInterface, 2, 3);

typedef struct {
    Scene base;
    MenuInterface interface;
} MenuScene;

void init_menu_scene(MenuScene *menu_scene, struct ncplane *parent_plane);
void enter_menu_scene(void *menu_scene);
void update_menu_scene(void *menu_scene);
void exit_menu_scene(void *menu_scene);
void free_menu_scene(MenuScene *menu_scene);
