#include "scenes/gameplay_scene.h"

#include <notcurses/notcurses.h>

#include "gameplay/world.h"
#include "interface/button.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    Vector parent_size = create_vector(columns, rows);

    const char *float_label = "000000.000";
    const char *int_label = "0000000000";

    init_scene(&gameplay_scene->base,
               &enter_gameplay_scene,
               &update_gameplay_scene,
               &draw_gameplay_scene,
               &exit_gameplay_scene,
               parent_size,
               parent_plane);

    init_text(&gameplay_scene->interface.texts[0],
              FPS_LABEL,
              strlen(FPS_LABEL),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(0, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[1],
              float_label,
              strlen(float_label),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(5, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[2],
              TICKS_LABEL,
              strlen(TICKS_LABEL),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(17, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[3],
              float_label,
              strlen(float_label),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(22, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[4],
              SCORE_LABEL,
              strlen(SCORE_LABEL),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(-21, 0),
              BOTTOM_RIGHT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[5],
              int_label,
              strlen(int_label),
              WHITE,
              HYPER_DARK_GRAY,
              create_vector(-16, 0),
              BOTTOM_RIGHT,
              parent_size);

    init_gameplay_interface(&gameplay_scene->interface, HYPER_DARK_GRAY, nullptr, 0, nullptr);
}

void enter_gameplay_scene(void *gameplay_scene) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;

    init_world(&scene->world, create_vector(512, 512));

    unsigned int rows;
    unsigned int columns;

    ncplane_dim_yx(scene->base.plane, &rows, &columns);
    Vector plane_size = create_vector(columns, rows);
    Vector view_size = create_vector(columns - 2, rows);

    init_view(&scene->view, create_vector(200, 200), view_size, plane_size, scene->base.plane);

    ncplane_move_yx(scene->base.plane, 0, 0);
}

void update_gameplay_scene(void *scene_context) {
    SceneContext *context = (SceneContext *)scene_context;
    GameplayScene *gameplay_scene = (GameplayScene *)context->current_scene;

    handle_interface_input(&gameplay_scene->interface.selected_button,
                           &gameplay_scene->interface.key_held_cooldown,
                           nullptr,
                           0,
                           gameplay_scene->interface.escape_handler,
                           gameplay_scene->interface.escape_handler_arg,
                           &context->input_state);

    handle_world_input(&gameplay_scene->world, &context->input_state);
    update_entities(&gameplay_scene->world);

    update_view_position(&gameplay_scene->view, &gameplay_scene->world, gameplay_scene->world.player.position);
}

void draw_gameplay_scene(void *gameplay_scene) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   nullptr,
                   0);

    draw_world_on_view(&scene->view, &scene->world);
}

void exit_gameplay_scene(void *gameplay_scene) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;

    free_view(&scene->view);
    free_world(&scene->world);

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_gameplay_scene(GameplayScene *gameplay_scene) {
    free_scene(&gameplay_scene->base);
    free_interface(gameplay_scene->interface.texts,
                   sizeof(gameplay_scene->interface.texts) / sizeof(Text),
                   nullptr,
                   0,
                   gameplay_scene->interface.escape_handler_arg);
    free_world(&gameplay_scene->world);
}
