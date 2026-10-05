#pragma once

#include "gameplay/view.h"
#include "gameplay/world.h"
#include "interface/interface.h"
#include "scene.h"
#include "types.h"

DEFINE_BUTTONLESS_INTERFACE(GameplayInterface, 6);
DEFINE_INIT_BUTTONLESS_INTERFACE(GameplayInterface, gameplay);

static const char *FPS_LABEL = "FPS:";

static const char *TICKS_LABEL = "TPS:";

static const char *SCORE_LABEL = "Score:";

struct GameplayScene {
    Scene base;
    GameplayInterface interface;
    World world;
    View view;
};

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context);
void enter_gameplay_scene(void *gameplay_scene);
void update_gameplay_scene(void *scene_context);
void draw_gameplay_scene(void *gameplay_scene);
void exit_gameplay_scene(void *gameplay_scene);
void free_gameplay_scene(GameplayScene *gameplay_scene);
