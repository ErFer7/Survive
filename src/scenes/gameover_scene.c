#include "scenes/gameover_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

void init_gameover_scene(GameoverScene *gameover_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    SceneTransition restart_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameplay_scene);
    SceneTransition menu_transition = create_scene_transition(scene_context, (Scene *)&scene_context->menu_scene);

    GameplaySceneArgs restart_world_args = create_gameplay_scene_args(RESTART, 0, 0, false);

    serialize_gameplay_scene_transition_args(&restart_world_args, restart_transition.args);

    const char *int_label = "0000000000";

    init_scene(&gameover_scene->base,
               &enter_gameover_scene,
               &update_gameover_scene,
               &draw_gameover_scene,
               &exit_gameover_scene,
               parent_size,
               parent_plane);

    init_text(&gameover_scene->interface.texts[0],
              GAMEOVER_TITLE,
              strlen(GAMEOVER_TITLE),
              RED,
              HYPER_DARK_GRAY,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(&gameover_scene->interface.texts[1],
              GAMEOVER_SCORE_LABEL,
              strlen(GAMEOVER_SCORE_LABEL),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(-6, 0),
              CENTER,
              parent_size);

    init_text(&gameover_scene->interface.texts[2],
              int_label,
              strlen(int_label),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(3, 0),
              CENTER,
              parent_size);

    init_button(&gameover_scene->interface.buttons[0],
                GAMEOVER_RESTART_BUTTON,
                strlen(GAMEOVER_RESTART_BUTTON),
                WHITE,
                HYPER_DARK_GRAY,
                create_vector(0, 2),
                CENTER,
                parent_size,
                &restart_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&gameover_scene->interface.buttons[1],
                MENU_BUTTON,
                strlen(MENU_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 4),
                CENTER,
                parent_size,
                &menu_transition,
                sizeof(SceneTransition),
                &transition);

    init_gameover_interface(&gameover_scene->interface,
                            HYPER_DARK_GRAY,
                            &menu_transition,
                            sizeof(SceneTransition),
                            &transition);
}

void enter_gameover_scene(void *gameover_scene, void *args) {
    GameoverScene *scene = (GameoverScene *)gameover_scene;

    uint16_t score;
    deserialize_gameover_scene_transition_args(&score, args);

    char score_str[11];

    snprintf(score_str, sizeof(score_str), "%010d", score);

    set_single_line_text_content(&scene->interface.texts[2], score_str, sizeof(score_str));

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_gameover_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    GameoverScene *gameover_scene = (GameoverScene *)context->current_scene;

    handle_interface_input(&gameover_scene->interface.selected_button,
                           &gameover_scene->interface.key_held_cooldown,
                           gameover_scene->interface.buttons,
                           sizeof(gameover_scene->interface.buttons) / sizeof(Button),
                           gameover_scene->interface.escape_handler,
                           gameover_scene->interface.escape_handler_arg,
                           &context->input_state);
}

void draw_gameover_scene(void *gameover_scene) {
    GameoverScene *scene = (GameoverScene *)gameover_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_gameover_scene(void *gameover_scene, void *args) {
    GameoverScene *scene = (GameoverScene *)gameover_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_gameover_scene(GameoverScene *gameover_scene) {
    free_scene(&gameover_scene->base);
    free_interface(gameover_scene->interface.texts,
                   sizeof(gameover_scene->interface.texts) / sizeof(Text),
                   gameover_scene->interface.buttons,
                   sizeof(gameover_scene->interface.buttons) / sizeof(Button),
                   gameover_scene->interface.escape_handler_arg);
}
