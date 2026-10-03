#pragma once

#include <notcurses/notcurses.h>

#include "scenes/info_scene.h"
#include "scenes/menu_scene.h"
#include "scenes/start_scene.h"
#include "system/periodic_thread.h"

struct SceneContext {
    Scene *current_scene;
    struct notcurses *not_curses;
    PeriodicThread update_thread;
    PeriodicThread render_thread;
    MenuScene menu_scene;
    StartScene start_scene;
    InfoScene info_scene;
};

struct SceneTransition {
    SceneContext *scene_context;
    Scene *next_scene;
};

void init_scene_context(SceneContext *scene_context);
void run(SceneContext *scene_context);
void update_scene(void *scene_context);
void render_scene(void *scene_context);
void transition(void *scene_transition);
void quit(void *scene_context);
void free_scene_context(SceneContext *scene_context);
