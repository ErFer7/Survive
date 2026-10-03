#include "scenes/info_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "utils/color.h"
#include "utils/vector.h"

void init_info_scene(InfoScene *info_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    SceneTransition menu_transition = {scene_context, (Scene *)&scene_context->menu_scene};

    init_scene(&info_scene->base,
               &enter_info_scene,
               &update_info_scene,
               &draw_info_scene,
               &exit_info_scene,
               parent_size,
               parent_plane);

    init_text(&info_scene->interface.texts[0],
              INFO_TITLE,
              strlen(INFO_TITLE),
              GREEN,
              HYPER_DARK_GRAY,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(&info_scene->interface.texts[1],
              INFO,
              strlen(INFO),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(0, 0),
              CENTER,
              parent_size);

    init_button(&info_scene->interface.buttons[0],
                INFO_BACK_BUTTON,
                strlen(INFO_BACK_BUTTON),
                GREEN,
                HYPER_DARK_GRAY,
                create_vector(0, 10),
                CENTER,
                parent_size,
                &menu_transition,
                sizeof(SceneTransition),
                &transition);

    init_info_interface(&info_scene->interface,
                        HYPER_DARK_GRAY,
                        &menu_transition,
                        sizeof(SceneTransition),
                        &transition);
}

void enter_info_scene(void *info_scene) {
    InfoScene *scene = (InfoScene *)info_scene;

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_info_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    InfoScene *info_scene = (InfoScene *)context->current_scene;

    handle_interface_input(&info_scene->interface.selected_button,
                           &info_scene->interface.key_held_cooldown,
                           info_scene->interface.buttons,
                           sizeof(info_scene->interface.buttons) / sizeof(Button),
                           info_scene->interface.escape_handler,
                           info_scene->interface.escape_handler_arg,
                           context->not_curses);
}

void draw_info_scene(void *info_scene) {
    InfoScene *scene = (InfoScene *)info_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_info_scene(void *info_scene) {
    InfoScene *scene = (InfoScene *)info_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_info_scene(InfoScene *info_scene) {
    free_scene(&info_scene->base);
    free_interface(info_scene->interface.texts,
                   sizeof(info_scene->interface.texts) / sizeof(Text),
                   info_scene->interface.buttons,
                   sizeof(info_scene->interface.buttons) / sizeof(Button),
                   info_scene->interface.escape_handler_arg);
}
