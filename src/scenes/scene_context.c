#include "scenes/scene_context.h"

#include <pthread.h>

#include "constants.h"
#include "scenes/menu_scene.h"
#include "system/periodic_thread.h"
#include "types.h"

void init_scene_context(SceneContext *scene_context) {
    notcurses_options nc_options;
    memset(&nc_options, 0, sizeof(notcurses_options));

    scene_context->not_curses = notcurses_init(&nc_options, stdout);
    struct ncplane *stdplane = notcurses_stdplane(scene_context->not_curses);

    init_periodic_thread(&scene_context->update_thread,
                         frequency_hz_to_period_ms(UPDATE_FREQUENCY),
                         &update_scene,
                         scene_context);

    init_periodic_thread(&scene_context->render_thread,
                         frequency_hz_to_period_ms(RENDER_FREQUENCY),
                         &render_scene,
                         scene_context);

    init_menu_scene(&scene_context->menu_scene, stdplane, scene_context);
    init_info_scene(&scene_context->info_scene, stdplane, scene_context);

    scene_context->current_scene = (Scene *)&scene_context->menu_scene;
    scene_context->current_scene->enter(scene_context->current_scene);
}

void run(SceneContext *scene_context) {
    start_periodic_thread(&scene_context->update_thread);
    start_periodic_thread(&scene_context->render_thread);

    join_periodic_thread(&scene_context->update_thread);
    join_periodic_thread(&scene_context->render_thread);
}

void update_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;

    context->current_scene->update(scene_context);
}

void render_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;

    context->current_scene->draw(context->current_scene);

    notcurses_render(context->not_curses);
}

void transition(void *scene_transition) { // FIX: aghjfah
    SceneTransition *transition = (SceneTransition *)scene_transition;
    SceneContext *scene_context = transition->scene_context;
    Scene *next_scene = transition->next_scene;

    scene_context->current_scene->exit(scene_context->current_scene);
    scene_context->current_scene = next_scene;
    scene_context->current_scene->enter(scene_context->current_scene);
}

void quit(void *scene_context) {
    SceneContext *context = *(SceneContext **)scene_context;

    stop_periodic_thread(&context->update_thread);
    stop_periodic_thread(&context->render_thread);
}

void free_scene_context(SceneContext *scene_context) {
    free_menu_scene(&scene_context->menu_scene);
    free_info_scene(&scene_context->info_scene);
    notcurses_stop(scene_context->not_curses);
    free_periodic_thread(&scene_context->update_thread);
    free_periodic_thread(&scene_context->render_thread);
}
