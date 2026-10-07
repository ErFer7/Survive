#pragma once

#include <notcurses/notcurses.h>

#include "scenes/gameplay_scene.h"
#include "scenes/info_scene.h"
#include "scenes/menu_scene.h"
#include "scenes/start_scene.h"
#include "system/input.h"
#include "system/periodic_thread.h"

#define TRANSITION_ARGS_SIZE 8

struct SceneContext {
    Scene *current_scene;
    struct notcurses *not_curses;
    pthread_mutex_t transition_render_mutex;
    PeriodicThread update_thread;
    PeriodicThread render_thread;
    InputState input_state;
    MenuScene menu_scene;
    StartScene start_scene;
    InfoScene info_scene;
    GameplayScene gameplay_scene;
};

struct SceneTransition {
    SceneContext *scene_context;
    Scene *next_scene;
    byte args[TRANSITION_ARGS_SIZE];
};

void init_scene_context(SceneContext *scene_context);
void run(SceneContext *scene_context);
void update_scene(void *scene_context);
void render_scene(void *scene_context);
void transition(void *scene_transition);
void quit(void *scene_context);
void free_scene_context(SceneContext *scene_context);

static inline SceneTransition create_scene_transition(SceneContext *scene_context, Scene *next_scene) {
    SceneTransition scene_transition = {scene_context, next_scene};
    memset(&scene_transition.args, 0, TRANSITION_ARGS_SIZE);

    return scene_transition;
}
