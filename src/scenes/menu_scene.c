#include "scenes/menu_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/colors.h"
#include "utils/vector.h"

void init_menu_scene(MenuScene *menu_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    SceneTransition start_transition = {scene_context, (Scene *)&scene_context->start_scene};
    SceneTransition info_transition = {scene_context, (Scene *)&scene_context->info_scene};

    init_scene(&menu_scene->base,
               &enter_menu_scene,
               &update_menu_scene,
               &draw_menu_scene,
               &exit_menu_scene,
               parent_size,
               parent_plane);

    init_text(&menu_scene->interface.texts[0],
              MAIN_MENU_TITLE,
              strlen(MAIN_MENU_TITLE),
              RED,
              HYPER_DARK_GRAY,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(&menu_scene->interface.texts[1],
              VERSION,
              strlen(VERSION),
              BLUE,
              HYPER_DARK_GRAY,
              create_vector(0, -1),
              BOTTOM,
              parent_size);

    init_button(&menu_scene->interface.buttons[0],
                START_BUTTON,
                strlen(START_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 0),
                CENTER,
                parent_size,
                &start_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&menu_scene->interface.buttons[1],
                INFO_BUTTON,
                strlen(INFO_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 2),
                CENTER,
                parent_size,
                &info_transition,
                sizeof(SceneTransition),
                &transition);

    init_button(&menu_scene->interface.buttons[2],
                QUIT_BUTTON,
                strlen(QUIT_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 4),
                CENTER,
                parent_size,
                &scene_context,
                sizeof(SceneContext *),
                &quit);

    init_menu_interface(&menu_scene->interface, HYPER_DARK_GRAY, &scene_context, sizeof(SceneContext *), &quit);
}

void enter_menu_scene(void *menu_scene) {
    MenuScene *scene = (MenuScene *)menu_scene;

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_menu_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    MenuScene *menu_scene = (MenuScene *)context->current_scene;

    handle_interface_input(&menu_scene->interface.selected_button,
                           &menu_scene->interface.key_held_cooldown,
                           menu_scene->interface.buttons,
                           sizeof(menu_scene->interface.buttons) / sizeof(Button),
                           menu_scene->interface.escape_handler,
                           menu_scene->interface.escape_handler_arg,
                           context->not_curses);
}

void draw_menu_scene(void *menu_scene) {
    MenuScene *scene = (MenuScene *)menu_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_menu_scene(void *menu_scene) {
    MenuScene *scene = (MenuScene *)menu_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_menu_scene(MenuScene *menu_scene) {
    free_scene(&menu_scene->base);
    free_interface(menu_scene->interface.texts,
                   sizeof(menu_scene->interface.texts) / sizeof(Text),
                   menu_scene->interface.buttons,
                   sizeof(menu_scene->interface.buttons) / sizeof(Button),
                   menu_scene->interface.escape_handler_arg);
}
