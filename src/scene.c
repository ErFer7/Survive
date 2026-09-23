#include "scene.h"

#include <pthread.h>
#include <stdlib.h>

void init_scene_context(SceneContext *scene_context) {
    scene_context->current_scene = nullptr;
    pthread_mutex_init(&scene_context->event_mutex, NULL);
}

void free_scene_context(SceneContext *scene_context) { pthread_mutex_destroy(&scene_context->event_mutex); }

void handle_event(SceneContext *scene_context, enum Event event) {
    pthread_mutex_lock(&scene_context->event_mutex);

    // TODO: Handle it here

    pthread_mutex_unlock(&scene_context->event_mutex);
}
