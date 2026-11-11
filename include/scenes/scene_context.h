#pragma once

#include <notcurses/notcurses.h>

#include "gameover_scene.h"
#include "gameplay_scene.h"
#include "info_scene.h"
#include "menu_scene.h"
#include "pause_scene.h"
#include "scene_transition.h"
#include "start_scene.h"
#include "system/input.h"
#include "system/periodic_thread.h"

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

    memset(&scene_transition->enter_args, 0, TRANSITION_ARGS_SIZE);
    memset(&scene_transition->exit_args, 0, TRANSITION_ARGS_SIZE);
}
