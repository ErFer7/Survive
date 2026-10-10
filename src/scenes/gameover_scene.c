#include "scenes/gameover_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

static const char *TITLE =
    " ██████╗  █████╗ ███╗   ███╗███████╗ ██████╗ ██╗   ██╗███████╗██████╗ \n"
    "██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔═══██╗██║   ██║██╔════╝██╔══██╗\n"
    "██║  ███╗███████║██╔████╔██║█████╗  ██║   ██║██║   ██║█████╗  ██████╔╝\n"
    "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n"
    "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n"
    " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝";

static const char *SCORE_LABEL = "Score:";

static const char *RESTART_BUTTON = " Restart ";

static const char *MENU_BUTTON = " Menu    ";

void init_gameover_scene(GameoverScene *gameover_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    VectorU parent_size = create_vector_u(columns, rows);

    SceneTransition restart_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameplay_scene);
    SceneTransition menu_transition = create_scene_transition(scene_context, (Scene *)&scene_context->menu_scene);

    GameplaySceneArgs restart_world_args = create_gameplay_scene_args(RESTART, false, VECTORU_ZERO);

    serialize_gameplay_scene_transition_args(&restart_world_args, restart_transition.args);

    const char *int_label = "0000000000";

    init_scene(&gameover_scene->base,
               &enter_gameover_scene,
               &update_gameover_scene,
               &draw_gameover_scene,
               &exit_gameover_scene,
               parent_size,
               DEFAULT_BACKGROUND,
               parent_plane);

    // TODO: Maybe there should be just a "create_text"
    init_text(&gameover_scene->interface.texts[0],
              TITLE,
              strlen(TITLE),
              RED,
              DEFAULT_BACKGROUND,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(&gameover_scene->interface.texts[1],
              SCORE_LABEL,
              strlen(SCORE_LABEL),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(-6, 0),
              CENTER,
              parent_size);

    init_text(&gameover_scene->interface.texts[2],
              int_label,
              strlen(int_label),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(3, 0),
              CENTER,
              parent_size);

    init_button(&gameover_scene->interface.buttons[0],
                RESTART_BUTTON,
                strlen(RESTART_BUTTON),
                WHITE,
                DEFAULT_BACKGROUND,
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
                DEFAULT_BACKGROUND,
                create_vector(0, 4),
                CENTER,
                parent_size,
                &menu_transition,
                sizeof(SceneTransition),
                &transition);

    init_gameover_interface(&gameover_scene->interface, &menu_transition, sizeof(SceneTransition), &transition);
}

void enter_gameover_scene(void *gameover_scene, void *args) {
    GameoverScene *scene = (GameoverScene *)gameover_scene;

    uint32_t score;
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
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_gameover_scene(void *gameover_scene, void *) {
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
