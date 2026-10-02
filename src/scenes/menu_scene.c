#include "scenes/menu_scene.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "interface/text.h"
#include "utils/colors.h"
#include "utils/vector.h"

static const char *MAIN_MENU_TITLE =
    "███████╗██╗   ██╗██████╗ ██╗   ██╗██╗██╗   ██╗███████╗\n"
    "██╔════╝██║   ██║██╔══██╗██║   ██║██║██║   ██║██╔════╝\n"
    "███████╗██║   ██║██████╔╝██║   ██║██║██║   ██║█████╗  \n"
    "╚════██║██║   ██║██╔══██╗╚██╗ ██╔╝██║╚██╗ ██╔╝██╔══╝  \n"
    "███████║╚██████╔╝██║  ██║ ╚████╔╝ ██║ ╚████╔╝ ███████╗\n"
    "╚══════╝ ╚═════╝ ╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═══╝  ╚══════╝";

static const char *VERSION = "v3.0";

static const char *START_BUTTON = " Start ";

static const char *INFO_BUTTON = " Info ";

static const char *QUIT_BUTTON = " Quit ";

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

    menu_scene->interface.background_color = HYPER_DARK_GRAY;  // FIX: This isn't working well

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
                nullptr,
                0,
                nullptr);

    init_button(&menu_scene->interface.buttons[1],
                INFO_BUTTON,
                strlen(INFO_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 2),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    init_button(&menu_scene->interface.buttons[2],
                QUIT_BUTTON,
                strlen(QUIT_BUTTON),
                RED,
                HYPER_DARK_GRAY,
                create_vector(0, 4),
                CENTER,
                parent_size,
                nullptr,
                0,
                nullptr);

    menu_scene->interface.selected_button = 0;
    toggle_selection(&menu_scene->interface.buttons[0]);
}

void enter_menu_scene(void *menu_scene) {}

void update_menu_scene(void *menu_scene) {
    MenuScene *scene = (MenuScene *)menu_scene;

    ncplane_set_bg_rgb(scene->base.plane, scene->interface.background_color);

    for (unsigned int i = 0; i < sizeof(scene->interface.texts) / sizeof(Text); i++) {
        draw_text(&scene->interface.texts[i], scene->base.plane);
    }

    for (unsigned int i = 0; i < sizeof(scene->interface.buttons) / sizeof(Button); i++) {
        draw_button(&scene->interface.buttons[i], scene->base.plane);
    }

    ncplane_set_bg_default(scene->base.plane);
}

void exit_menu_scene(void *menu_scene) {}
