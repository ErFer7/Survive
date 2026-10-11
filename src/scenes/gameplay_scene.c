#include "scenes/gameplay_scene.h"

#include <notcurses/notcurses.h>

#include "gameplay/gameplay.h"
#include "interface/text.h"
#include "scenes/scene_context.h"
#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

static const char *FPS_LABEL = "FPS:";

static const char *TICKS_LABEL = "TPS:";

static const char *SCORE_LABEL = "Score:";

void init_gameplay_scene(GameplayScene *gameplay_scene, struct ncplane *parent_plane, SceneContext *scene_context) {
    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(parent_plane, &rows, &columns);
    VectorU parent_size = create_vector_u(columns, rows);

    SceneTransition pause_transition = create_scene_transition(scene_context, (Scene *)&scene_context->pause_scene);
    SceneTransition gameover_transition =
        create_scene_transition(scene_context, (Scene *)&scene_context->gameover_scene);

    GameplaySceneArgs pause_transition_args = create_gameplay_scene_args(PAUSE, false, VECTORU_ZERO);

    serialize_gameplay_scene_transition_args(&pause_transition_args, pause_transition.args);

    const char *float_label = "000000.000";
    const char *int_label = "0000000000";

    init_scene(&gameplay_scene->base,
               gameplay_scene,
               &enter_gameplay_scene,
               &update_gameplay_scene,
               &draw_gameplay_scene,
               &exit_gameplay_scene,
               &gameplay_resize_callback,
               parent_size,
               DEFAULT_BACKGROUND,
               parent_plane);

    init_text(&gameplay_scene->interface.texts[0],
              FPS_LABEL,
              strlen(FPS_LABEL),
              WHITE,
              DEFAULT_BACKGROUND,
              VECTOR_ZERO,
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[1],
              float_label,
              strlen(float_label),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(5, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[2],
              TICKS_LABEL,
              strlen(TICKS_LABEL),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(17, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[3],
              float_label,
              strlen(float_label),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(22, 0),
              BOTTOM_LEFT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[4],
              SCORE_LABEL,
              strlen(SCORE_LABEL),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(-21, 0),
              BOTTOM_RIGHT,
              parent_size);

    init_text(&gameplay_scene->interface.texts[5],
              int_label,
              strlen(int_label),
              WHITE,
              DEFAULT_BACKGROUND,
              create_vector(-16, 0),
              BOTTOM_RIGHT,
              parent_size);

    init_gameplay_interface(&gameplay_scene->interface, &pause_transition, sizeof(SceneTransition), &transition);

    init_gameplay(&gameplay_scene->gameplay,
                  vector_to_vector_u(add_vector(vector_u_to_vector(parent_size), VIEW_SIZE_OFFSET)),
                  parent_size,
                  &gameover_transition,
                  &gameplay_scene->interface.texts[5],
                  gameplay_scene->base.plane);

    scene_context->render_thread.frequency_info = &gameplay_scene->interface.texts[1];
    scene_context->update_thread.frequency_info = &gameplay_scene->interface.texts[3];
}

void enter_gameplay_scene(void *gameplay_scene, void *args) {
    GameplayScene *scene = (GameplayScene *)gameplay_scene;
    GameplaySceneArgs scene_args;

    deserialize_gameplay_scene_transition_args(&scene_args, args);

    if (scene_args.transition_mode == START || scene_args.transition_mode == RESTART) {
        start_gameplay(&scene->gameplay,
                       scene_args.transition_mode == RESTART,
                       scene_args.world_size,
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

int gameplay_resize_callback(struct ncplane *parent_plane) {
    GameplayScene *gameplay_scene = (GameplayScene *)ncplane_userptr(parent_plane);

    uint32_t rows;
    uint32_t columns;

    ncplane_dim_yx(gameplay_scene->base.parent_plane, &rows, &columns);
    VectorU parent_size = create_vector_u(columns, rows);

    resize_scene(&gameplay_scene->base, parent_size);

    resize_interface(gameplay_scene->interface.texts,
                     sizeof(gameplay_scene->interface.texts) / sizeof(Text),
                     nullptr,
                     0,
                     parent_size);

    resize_gameplay_view(&gameplay_scene->gameplay, parent_size);

    ncplane_erase(gameplay_scene->base.plane);

    return 0;
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
