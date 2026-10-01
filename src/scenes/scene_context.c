#include "scenes/scene_context.h"

#include <pthread.h>

#include "scenes/menu_scene.h"

void init_scene_context(SceneContext *scene_context, struct ncplane *stdplane) {
    scene_context->current_scene = (Scene *)&scene_context->menu_scene;
    pthread_mutex_init(&scene_context->event_mutex, NULL);

    init_menu_scene(&scene_context->menu_scene, stdplane);
}

void handle_event(SceneContext *scene_context, enum Event event) {
    pthread_mutex_lock(&scene_context->event_mutex);

    // TODO: Handle it here

    pthread_mutex_unlock(&scene_context->event_mutex);
}

void update_scene(SceneContext *scene_context) {
    if (scene_context->current_scene->update != nullptr) {
        scene_context->current_scene->update(scene_context->current_scene);
    }
}

int is_exiting(SceneContext *scene_context) { return scene_context->current_scene == nullptr ? 1 : 0; }

void free_scene_context(SceneContext *scene_context) { pthread_mutex_destroy(&scene_context->event_mutex); }
