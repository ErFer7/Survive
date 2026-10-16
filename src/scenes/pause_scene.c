#include "scenes/pause_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

static const char *TITLE =
    "██████╗  █████╗ ██╗   ██╗███████╗███████╗\n"
    "██╔══██╗██╔══██╗██║   ██║██╔════╝██╔════╝\n"
    "██████╔╝███████║██║   ██║███████╗█████╗  \n"
    "██╔═══╝ ██╔══██║██║   ██║╚════██║██╔══╝  \n"
    "██║     ██║  ██║╚██████╔╝███████║███████╗\n"
    "╚═╝     ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝";

static const char *RESUME_BUTTON = " Resume  ";

static const char *RESTART_BUTTON = " Restart ";

static const char *GAMEOVER_BUTTON = " Leave   ";

void init_pause_scene(PauseScene *pause_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    VectorU parent_size = VECTOR_U(columns, rows);

    SceneTransition resume_transition;
    SceneTransition restart_transition;
    SceneTransition gameover_transition;

    init_scene_transition(&resume_transition, scene_context, (Scene *)&scene_context->gameplay_scene);
    init_scene_transition(&restart_transition, scene_context, (Scene *)&scene_context->gameplay_scene);

    // FIX: Update the score when leaving too
    init_scene_transition(&gameover_transition, scene_context, (Scene *)&scene_context->gameover_scene);

    GameplaySceneArgs resume_world_args;
    GameplaySceneArgs restart_world_args;

    init_gameplay_scene_args(&resume_world_args, RESUME, false, false, VECTORU_ZERO);
    init_gameplay_scene_args(&restart_world_args, RESTART, false, false, VECTORU_ZERO);

    serialize_gameplay_scene_transition_args(&resume_world_args, resume_transition.args);
    serialize_gameplay_scene_transition_args(&restart_world_args, restart_transition.args);

    init_scene(&pause_scene->base,
               pause_scene,
               &enter_pause_scene,
               &update_pause_scene,
               &draw_pause_scene,
               &exit_pause_scene,
               &pause_resize_callback,
               parent_size,
               DEFAULT_BACKGROUND,
               parent_plane);

    init_text(&pause_scene->interface.texts[0],
              TITLE,
              strlen(TITLE),
              WHITE,
              DEFAULT_BACKGROUND,
              VECTOR(0, 4),
              TOP,
              parent_size);

    init_button(&pause_scene->interface.buttons[0],
                RESUME_BUTTON,
                strlen(RESUME_BUTTON),
                WHITE,
                DEFAULT_BACKGROUND,
                VECTOR_ZERO,
                CENTER,
                parent_size,
                &resume_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&pause_scene->interface.buttons[1],
                RESTART_BUTTON,
                strlen(RESTART_BUTTON),
                WHITE,
                DEFAULT_BACKGROUND,
                VECTOR(0, 2),
                CENTER,
                parent_size,
                &restart_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&pause_scene->interface.buttons[2],
                GAMEOVER_BUTTON,
                strlen(GAMEOVER_BUTTON),
                RED,
                DEFAULT_BACKGROUND,
                VECTOR(0, 4),
                CENTER,
                parent_size,
                &gameover_transition,
                sizeof(SceneTransition),
                &transition);

    init_pause_interface(&pause_scene->interface, &resume_transition, sizeof(SceneTransition), &transition);
}

void enter_pause_scene(void *pause_scene, void *) {
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
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_pause_scene(void *pause_scene, void *) {
    PauseScene *scene = (PauseScene *)pause_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

int pause_resize_callback(struct ncplane *parent_plane) {
    PauseScene *pause_scene = (PauseScene *)ncplane_userptr(parent_plane);

    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(pause_scene->base.parent_plane, &rows, &columns);
    VectorU parent_size = VECTOR_U(columns, rows);

    resize_scene(&pause_scene->base, parent_size);

    resize_interface(pause_scene->interface.texts,
                     sizeof(pause_scene->interface.texts) / sizeof(Text),
                     pause_scene->interface.buttons,
                     sizeof(pause_scene->interface.buttons) / sizeof(Button),
                     parent_size);

    ncplane_erase(pause_scene->base.plane);

    return 0;
}

void free_pause_scene(PauseScene *pause_scene) {
    free_scene(&pause_scene->base);
    free_interface(pause_scene->interface.texts,
                   sizeof(pause_scene->interface.texts) / sizeof(Text),
                   pause_scene->interface.buttons,
                   sizeof(pause_scene->interface.buttons) / sizeof(Button),
                   pause_scene->interface.escape_handler_arg);
}
