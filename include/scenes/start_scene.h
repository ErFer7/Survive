#pragma once

#include "interface/interface.h"
#include "scene.h"

DEFINE_INTERFACE(StartInterface, 3, 6);
DEFINE_INIT_INTERFACE(StartInterface, start);

static const char *START_TITLE =
    "███████╗████████╗ █████╗ ██████╗ ████████╗\n"
    "██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝\n"
    "███████╗   ██║   ███████║██████╔╝   ██║   \n"
    "╚════██║   ██║   ██╔══██║██╔══██╗   ██║   \n"
    "███████║   ██║   ██║  ██║██║  ██║   ██║   \n"
    "╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝";

static const char *START_INFO = "Choose your game mode and world size.";

static const char *WARNING = "Large worlds can use a lot of memory!";

static const char *SMALL_BUTTON = " Small ";

static const char *REGULAR_BUTTON = " Regular ";

static const char *LARGE_BUTTON = " Large ";

static const char *MEGA_BUTTON = " MEGA ";

static const char *CLASSIC_BUTTON = " Classic ";

static const char *START_BACK_BUTTON = " Back ";

struct StartScene {
    Scene base;
    StartInterface interface;
};

void init_start_scene(StartScene *start_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_start_scene(void *start_scene);
void update_start_scene(void *scene_context);
void draw_start_scene(void *start_scene);
void exit_start_scene(void *start_scene);
void free_start_scene(StartScene *start_scene);
