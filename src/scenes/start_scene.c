#include "scenes/start_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "utils/color.h"
#include "utils/vector.h"

void init_start_scene(StartScene *start_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    SceneTransition menu_transition = {scene_context, (Scene *)&scene_context->menu_scene};

    init_scene(&start_scene->base,
               &enter_start_scene,
               &update_start_scene,
               &draw_start_scene,
               &exit_start_scene,
               parent_size,
               parent_plane);

    init_text(&start_scene->interface.texts[0],
              START_TITLE,
              strlen(START_TITLE),
              RED,
              HYPER_DARK_GRAY,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(&start_scene->interface.texts[1],
              START_INFO,
              strlen(START_INFO),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(0, 0),
              CENTER,
              parent_size);

    init_text(&start_scene->interface.texts[2],
              WARNING,
              strlen(WARNING),
              YELLOW,
              HYPER_DARK_GRAY,
              create_vector(0, 1),
              CENTER,
              parent_size);

    init_button(&start_scene->interface.buttons[0],
                SMALL_BUTTON,
                strlen(SMALL_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 3),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&start_scene->interface.buttons[1],
                REGULAR_BUTTON,
                strlen(REGULAR_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 4),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&start_scene->interface.buttons[2],
                LARGE_BUTTON,
                strlen(LARGE_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 5),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&start_scene->interface.buttons[3],
                MEGA_BUTTON,
                strlen(MEGA_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 6),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&start_scene->interface.buttons[4],
                CLASSIC_BUTTON,
                strlen(CLASSIC_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 7),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&start_scene->interface.buttons[5],
                START_BACK_BUTTON,
                strlen(START_BACK_BUTTON),
                WHITE,
                HYPER_DARK_GRAY,
                create_vector(0, 9),
                CENTER,
                parent_size,
                &menu_transition,
                sizeof(SceneTransition),
                &transition);

    init_start_interface(&start_scene->interface,
                         HYPER_DARK_GRAY,
                         &menu_transition,
                         sizeof(SceneTransition),
                         &transition);
}

void enter_start_scene(void *start_scene) {
    StartScene *scene = (StartScene *)start_scene;

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_start_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    StartScene *start_scene = (StartScene *)context->current_scene;

    handle_interface_input(&start_scene->interface.selected_button,
                           &start_scene->interface.key_held_cooldown,
                           start_scene->interface.buttons,
                           sizeof(start_scene->interface.buttons) / sizeof(Button),
                           start_scene->interface.escape_handler,
                           start_scene->interface.escape_handler_arg,
                           context->not_curses);
}

void draw_start_scene(void *start_scene) {
    StartScene *scene = (StartScene *)start_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_start_scene(void *start_scene) {
    StartScene *scene = (StartScene *)start_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_start_scene(StartScene *start_scene) {
    free_scene(&start_scene->base);
    free_interface(start_scene->interface.texts,
                   sizeof(start_scene->interface.texts) / sizeof(Text),
                   start_scene->interface.buttons,
                   sizeof(start_scene->interface.buttons) / sizeof(Button),
                   start_scene->interface.escape_handler_arg);
}
