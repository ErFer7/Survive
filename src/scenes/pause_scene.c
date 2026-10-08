#include "scenes/pause_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

void init_pause_scene(PauseScene *pause_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    SceneTransition resume_transition = create_scene_transition(scene_context, (Scene *)&scene_context->gameplay_scene);
    SceneTransition restart_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameplay_scene);
    SceneTransition gameover_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameover_scene);

    GameplaySceneArgs resume_world_args = create_gameplay_scene_args(RESUME, 0, 0, false);
    GameplaySceneArgs restart_world_args = create_gameplay_scene_args(RESTART, 0, 0, false);

    // SceneTransition gameover_transition = create_scene_transition(scene_context, (Scene
    // *)&scene_context->info_scene);

    serialize_gameplay_scene_transition_args(&resume_world_args, resume_transition.args);
    serialize_gameplay_scene_transition_args(&restart_world_args, restart_transition.args);

    init_scene(&pause_scene->base,
               &enter_pause_scene,
               &update_pause_scene,
               &draw_pause_scene,
               &exit_pause_scene,
               parent_size,
               parent_plane);

    init_text(&pause_scene->interface.texts[0],
              PAUSE_TITLE,
              strlen(PAUSE_TITLE),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_button(&pause_scene->interface.buttons[0],
                RESUME_BUTTON,
                strlen(RESUME_BUTTON),
                WHITE,
                HYPER_DARK_GRAY,
                create_vector(0, 0),
                CENTER,
                parent_size,
                &resume_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&pause_scene->interface.buttons[1],
                PAUSE_RESTART_BUTTON,
                strlen(PAUSE_RESTART_BUTTON),
                WHITE,
                HYPER_DARK_GRAY,
                create_vector(0, 2),
                CENTER,
                parent_size,
                &restart_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&pause_scene->interface.buttons[2],
                GAMEOVER_BUTTON,
                strlen(GAMEOVER_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 4),
                CENTER,
                parent_size,
                &gameover_transition,
                sizeof(SceneTransition),
                &transition);

    init_pause_interface(&pause_scene->interface,
                         HYPER_DARK_GRAY,
                         &resume_transition,
                         sizeof(SceneTransition),
                         &transition);
}

void enter_pause_scene(void *pause_scene, void *args) {
    PauseScene *scene = (PauseScene *)pause_scene;

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_pause_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    PauseScene *pause_scene = (PauseScene *)context->current_scene;

    handle_interface_input(&pause_scene->interface.selected_button,
                           &pause_scene->interface.key_held_cooldown,
                           pause_scene->interface.buttons,
                           sizeof(pause_scene->interface.buttons) / sizeof(Button),
                           pause_scene->interface.escape_handler,
                           pause_scene->interface.escape_handler_arg,
                           &context->input_state);
}

void draw_pause_scene(void *pause_scene) {
    PauseScene *scene = (PauseScene *)pause_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_pause_scene(void *pause_scene, void *args) {
    PauseScene *scene = (PauseScene *)pause_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_pause_scene(PauseScene *pause_scene) {
    free_scene(&pause_scene->base);
    free_interface(pause_scene->interface.texts,
                   sizeof(pause_scene->interface.texts) / sizeof(Text),
                   pause_scene->interface.buttons,
                   sizeof(pause_scene->interface.buttons) / sizeof(Button),
                   pause_scene->interface.escape_handler_arg);
}
