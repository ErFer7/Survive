#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(GameoverInterface, 3, 2);
DEFINE_INIT_INTERFACE(GameoverInterface, gameover);

static const char *GAMEOVER_TITLE =
    " ██████╗  █████╗ ███╗   ███╗███████╗ ██████╗ ██╗   ██╗███████╗██████╗ \n"
    "██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔═══██╗██║   ██║██╔════╝██╔══██╗\n"
    "██║  ███╗███████║██╔████╔██║█████╗  ██║   ██║██║   ██║█████╗  ██████╔╝\n"
    "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n"
    "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n"
    " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝";

static const char *GAMEOVER_SCORE_LABEL = "Score:";

static const char *GAMEOVER_RESTART_BUTTON = " Restart ";

static const char *MENU_BUTTON = " Menu ";

struct GameoverScene {
    Scene base;
    GameoverInterface interface;
};

void init_gameover_scene(GameoverScene *gameover_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameover_scene(void *gameover_scene, void *args);
void update_gameover_scene(void *scene_context);
void draw_gameover_scene(void *gameover_scene);
void exit_gameover_scene(void *gameover_scene, void *args);
void free_gameover_scene(GameoverScene *gameover_scene);
