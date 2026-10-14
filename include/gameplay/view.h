#pragma once

#include <notcurses/notcurses.h>

#include "gameplay/cell_matrix.h"
#include "types.h"
#include "utils/color.h"
#include "utils/utf8.h"
#include "utils/vector.h"

#define COOL_EFFECTS

static const Vector VIEW_SIZE_OFFSET = {-2, -2};

static const float ANIMATION_SPEED = 8.0f;

#ifdef COOL_EFFECTS
static const float VIEW_EFFECTS_ANGLE_STEP = 0.001f;
static const float VIEW_PERLIN_NOISE_SCALE = 0.1f;
static const float VIEW_PERLIN_NOISE_PERSISTENCE = 1.75f;
static const int32_t VIEW_PERLIN_NOISE_OCTAVES = 5;
static const utf8_char VIEW_EFFECTS_CHARACTER[1] = {UTF8("█")};
static const uint32_t VIEW_EFFECTS_CELL_ANIMATION_LENGTH = 1;
static const enum Color VIEW_MAX_OUTSIZE_COLOR = BRIGHT_CYAN;
#endif

struct View {
    Vector position;
    struct ncplane *plane;
    struct ncplane_options plane_options;
    struct ncplane *parent_plane;
#ifdef COOL_EFFECTS
    CellMatrix cell_matrix;
    float perlin_seed;
    float angle;
#endif
};

void init_view(View *view,
               Vector initial_position,
               VectorU size,
               VectorU parent_size,
               enum Color background_color,
               struct ncplane *parent_plane);
void draw_world_on_view(View *view, World *world);
#ifdef COOL_EFFECTS
void generate_effects(View *view, VectorU size);
#endif
void free_view(View *view);

static inline void resize_view(View *view, VectorU size) {
    ncplane_resize_simple(view->plane, size.y + VIEW_SIZE_OFFSET.y, size.x + VIEW_SIZE_OFFSET.x);
    generate_effects(view, size);
}
