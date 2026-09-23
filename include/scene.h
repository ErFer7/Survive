#pragma once

#include <pthread.h>

#include "events.h"

typedef struct _Interface Scene;

typedef struct {
    Scene *current_scene;
    pthread_mutex_t event_mutex;
} SceneContext;

void init_scene_context(SceneContext *scene_context);
void free_scene_context(SceneContext *scene_context);

void handle_event(SceneContext *scene_context, enum Event event);
