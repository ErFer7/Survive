#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(InfoInterface, 2, 1);
DEFINE_INIT_INTERFACE(InfoInterface, info);

static const char *INFO_TITLE =
    "██╗███╗   ██╗███████╗ ██████╗ \n"
    "██║████╗  ██║██╔════╝██╔═══██╗\n"
    "██║██╔██╗ ██║█████╗  ██║   ██║\n"
    "██║██║╚██╗██║██╔══╝  ██║   ██║\n"
    "██║██║ ╚████║██║     ╚██████╔╝\n"
    "╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝";

static const char *INFO =
    "Adaptation of my first game that was created in 2019-03-19.\n"
    "Written in C 💙.\n \n"
    "Use the arrows to control the player and press X to run.\n \n"
    "~Hefer\n \n"
    "https://github.com/ErFer7/Survive";

static const char *INFO_BACK_BUTTON = " Back ";

struct InfoScene {
    Scene base;
    InfoInterface interface;
};

void init_info_scene(InfoScene *info_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_info_scene(void *info_scene, void *args);
void update_info_scene(void *scene_context);
void draw_info_scene(void *info_scene);
void exit_info_scene(void *info_scene);
void free_info_scene(InfoScene *info_scene);
