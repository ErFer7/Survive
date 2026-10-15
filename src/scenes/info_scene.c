#include "scenes/info_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "utils/color.h"
#include "utils/vector.h"

static const char *TITLE =
    "██╗███╗   ██╗███████╗ ██████╗ \n"
    "██║████╗  ██║██╔════╝██╔═══██╗\n"
    "██║██╔██╗ ██║█████╗  ██║   ██║\n"
    "██║██║╚██╗██║██╔══╝  ██║   ██║\n"
    "██║██║ ╚████║██║     ╚██████╔╝\n"
    "╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝";

static const char *INFO =
    "This is an adaptation of my first game that was created in 2019-03-19.\n"
    "Written in C 💙.\n \n"
    "Use the arrows to control the player. Press shift to slow down and ctrl to run.\n \n"
    "Author: ~Hefer\n \n"
    "Repository: https://github.com/ErFer7/Survive";

static const char *BACK_BUTTON = " Back ";

void init_info_scene(InfoScene *info_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    VectorU parent_size = VECTOR_U(columns, rows);

    SceneTransition menu_transition;

    init_scene_transition(&menu_transition, scene_context, (Scene *)&scene_context->menu_scene);

    init_scene(&info_scene->base,
               info_scene,
               &enter_info_scene,
               &update_info_scene,
               &draw_info_scene,
               &exit_info_scene,
               &info_resize_callback,
               parent_size,
               DEFAULT_BACKGROUND,
               parent_plane);

    init_text(&info_scene->interface.texts[0],
              TITLE,
              strlen(TITLE),
              GREEN,
              DEFAULT_BACKGROUND,
              VECTOR(0, 4),
              TOP,
              parent_size);

    init_text(&info_scene->interface.texts[1],
              INFO,
              strlen(INFO),
              WHITE,
              DEFAULT_BACKGROUND,
              VECTOR_ZERO,
              CENTER,
              parent_size);

    init_button(&info_scene->interface.buttons[0],
                BACK_BUTTON,
                strlen(BACK_BUTTON),
                GREEN,
                DEFAULT_BACKGROUND,
                VECTOR(0, 10),
                CENTER,
                parent_size,
                &menu_transition,
                sizeof(SceneTransition),
                &transition);

    init_info_interface(&info_scene->interface, &menu_transition, sizeof(SceneTransition), &transition);
}

void enter_info_scene(void *info_scene, void *) {
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
                           &context->input_state);
}

void draw_info_scene(void *info_scene) {
    InfoScene *scene = (InfoScene *)info_scene;

    draw_interface(scene->base.plane,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   scene->interface.buttons,
                   sizeof(scene->interface.buttons) / sizeof(Button));
}

void exit_info_scene(void *info_scene, void *) {
    InfoScene *scene = (InfoScene *)info_scene;

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

int info_resize_callback(struct ncplane *parent_plane) {
    InfoScene *info_scene = (InfoScene *)ncplane_userptr(parent_plane);

    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(info_scene->base.parent_plane, &rows, &columns);
    VectorU parent_size = VECTOR_U(columns, rows);

    resize_scene(&info_scene->base, parent_size);

    resize_interface(info_scene->interface.texts,
                     sizeof(info_scene->interface.texts) / sizeof(Text),
                     info_scene->interface.buttons,
                     sizeof(info_scene->interface.buttons) / sizeof(Button),
                     parent_size);

    ncplane_erase(info_scene->base.plane);

    return 0;
}

void free_info_scene(InfoScene *info_scene) {
    free_scene(&info_scene->base);
    free_interface(info_scene->interface.texts,
                   sizeof(info_scene->interface.texts) / sizeof(Text),
                   info_scene->interface.buttons,
                   sizeof(info_scene->interface.buttons) / sizeof(Button),
                   info_scene->interface.escape_handler_arg);
}
