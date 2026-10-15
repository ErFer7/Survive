#pragma once

#include <notcurses/notcurses.h>

#include "scenes/gameover_scene.h"
#include "scenes/gameplay_scene.h"
#include "scenes/info_scene.h"
#include "scenes/menu_scene.h"
#include "scenes/pause_scene.h"
#include "scenes/start_scene.h"
#include "system/input.h"
#include "system/periodic_thread.h"

#define TRANSITION_ARGS_SIZE 16

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
    PauseScene pause_scene;
    GameoverScene gameover_scene;
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

static inline void init_scene_transition(SceneTransition *scene_transition,
                                         SceneContext *scene_context,
                                         Scene *next_scene) {
    scene_transition->scene_context = scene_context;
    scene_transition->next_scene = next_scene;

    memset(&scene_transition->args, 0, TRANSITION_ARGS_SIZE);
}
