#pragma once

#include <notcurses/notcurses.h>
#include <pthread.h>

#include "../events.h"
#include "menu_scene.h"
#include "scene.h"

typedef struct {
    Scene *current_scene;
    pthread_mutex_t event_mutex;
    MenuScene menu_scene;
} SceneContext;

void init_scene_context(SceneContext *scene_context, struct ncplane *stdplane);
void handle_event(SceneContext *scene_context, enum Event event);
void update_scene(SceneContext *scene_context);
int is_exiting(SceneContext *scene_context);
void free_scene_context(SceneContext *scene_context);
