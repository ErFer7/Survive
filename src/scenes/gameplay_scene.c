#include "scenes/gameplay_scene.h"

#include <notcurses/notcurses.h>

#include "gameplay/gameplay.h"
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

    SceneTransition pause_transition = create_scene_transition(scene_context, (Scene *)&scene_context->pause_scene);
    SceneTransition gameover_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameover_scene);

    GameplaySceneArgs pause_transition_args = create_gameplay_scene_args(PAUSE, 0, 0, false);

    serialize_gameplay_scene_transition_args(&pause_transition_args, pause_transition.args);

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
              VECTOR_ZERO,
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
              GAMEPLAY_SCORE_LABEL,
              strlen(GAMEPLAY_SCORE_LABEL),
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

    init_gameplay_interface(&gameplay_scene->interface,
                            HYPER_DARK_GRAY,
                            &pause_transition,
                            sizeof(SceneTransition),
                            &transition);

    init_gameplay(&gameplay_scene->gameplay,
                  create_vector(columns - 2, rows - 2),
                  parent_size,
                  &gameover_transition,
                  gameplay_scene->base.plane);
}

void enter_gameplay_scene(void *gameplay_scene, void *args) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;
    GameplaySceneArgs scene_args;

    deserialize_gameplay_scene_transition_args(&scene_args, args);

    // TODO: Clean up these logic checks
    if (scene_args.transition_mode == START || scene_args.transition_mode == RESTART) {
        start_gameplay(&scene->gameplay,
                       scene_args.transition_mode == RESTART,
                       create_vector(scene_args.world_width, scene_args.world_height),
                       scene_args.enable_terrain_generation);
    }

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

    update_gameplay(&gameplay_scene->gameplay, &context->input_state);
}

void draw_gameplay_scene(void *gameplay_scene) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;

    draw_interface(scene->base.plane,
                   scene->interface.background_color,
                   scene->interface.texts,
                   sizeof(scene->interface.texts) / sizeof(Text),
                   nullptr,
                   0);

    draw_gameplay(&scene->gameplay);
}

void exit_gameplay_scene(void *gameplay_scene, void *args) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;
    GameplaySceneArgs scene_args;

    deserialize_gameplay_scene_transition_args(&scene_args, args);

    if (scene_args.transition_mode != PAUSE) {
        partially_free_gameplay(&scene->gameplay);
    }

    ncplane_move_yx(scene->base.plane, -9999, -9999);
}

void free_gameplay_scene(GameplayScene *gameplay_scene) {
    free_scene(&gameplay_scene->base);
    free_interface(gameplay_scene->interface.texts,
                   sizeof(gameplay_scene->interface.texts) / sizeof(Text),
                   nullptr,
                   0,
                   gameplay_scene->interface.escape_handler_arg);
    free_gameplay(&gameplay_scene->gameplay);
}
