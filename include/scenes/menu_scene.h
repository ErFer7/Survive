#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(MenuInterface, 2, 3);
DEFINE_INIT_INTERFACE(MenuInterface, menu);

static const char *MAIN_MENU_TITLE =
    "███████╗██╗   ██╗██████╗ ██╗   ██╗██╗██╗   ██╗███████╗\n"
    "██╔════╝██║   ██║██╔══██╗██║   ██║██║██║   ██║██╔════╝\n"
    "███████╗██║   ██║██████╔╝██║   ██║██║██║   ██║█████╗  \n"
    "╚════██║██║   ██║██╔══██╗╚██╗ ██╔╝██║╚██╗ ██╔╝██╔══╝  \n"
    "███████║╚██████╔╝██║  ██║ ╚████╔╝ ██║ ╚████╔╝ ███████╗\n"
    "╚══════╝ ╚═════╝ ╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═══╝  ╚══════╝";

static const char *VERSION = "v3.0";

static const char *START_BUTTON = " Start ";

static const char *INFO_BUTTON = " Info ";

static const char *QUIT_BUTTON = " Quit ";

struct MenuScene {
    Scene base;
    MenuInterface interface;
};

void init_menu_scene(MenuScene *menu_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_menu_scene(void *menu_scene);
void update_menu_scene(void *scene_context);
void draw_menu_scene(void *menu_scene);
void exit_menu_scene(void *menu_scene);
void free_menu_scene(MenuScene *menu_scene);
