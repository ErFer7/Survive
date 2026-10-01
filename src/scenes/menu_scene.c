#include "scenes/menu_scene.h"

#include <notcurses/notcurses.h>

#include "interface/text.h"
#include "interface/texts.h"
#include "utils/colors.h"
#include "utils/vector.h"

void init_menu_scene(MenuScene *menu_scene, struct ncplane *parent_plane) {
    menu_scene->base.enter = &enter_menu_scene;
    menu_scene->base.update = &update_menu_scene;
    menu_scene->base.exit = &enter_menu_scene;
    menu_scene->base.plane_options.x = 0;
    menu_scene->base.plane_options.y = 0;
    menu_scene->base.plane_options.userptr = nullptr;
    menu_scene->base.plane_options.name = nullptr;
    menu_scene->base.plane_options.resizecb = nullptr;
    menu_scene->base.plane_options.flags = NCPLANE_OPTION_FIXED;
    menu_scene->base.plane_options.margin_b = 0;
    menu_scene->base.plane_options.margin_r = 0;

    ncplane_dim_yx(parent_plane, &menu_scene->base.plane_options.rows, &menu_scene->base.plane_options.cols);

    menu_scene->base.plane = ncplane_create(parent_plane, &menu_scene->base.plane_options);

    unsigned rows, cols;

    ncplane_dim_yx(parent_plane, &rows, &cols);

    Vector parent_size = create_vector(cols, rows);

    init_text(&menu_scene->interface.texts[0],
              MAIN_MENU_TITLE,
              strlen(MAIN_MENU_TITLE),
              RED,
              create_vector(0, 4),
              TOP,
              parent_size);

    init_text(
        &menu_scene->interface.texts[1], VERSION, strlen(VERSION), BLUE, create_vector(0, -1), BOTTOM, parent_size);
}

void enter_menu_scene(void *menu_scene) {}

void update_menu_scene(void *menu_scene) {
    MenuScene *scene = (MenuScene *)menu_scene;

    for (unsigned int i = 0; i < sizeof(scene->interface.texts) / sizeof(Text); i++) {
        draw_text(&scene->interface.texts[i], scene->base.plane);
    }
}

void exit_menu_scene(void *menu_scene) {}
