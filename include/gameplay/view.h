#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"
#include "utils/utf8.h"
#include "utils/vector.h"

#define COOL_EFFECTS

#ifdef COOL_EFFECTS
static const float VIEW_EFFECTS_PARALLAX = 1.25f;
static const float VIEW_PERLIN_NOISE_SCALE = 0.1f;
static const float VIEW_PERLIN_NOISE_PERSISTENCE = 1.75f;
static const int32_t VIEW_PERLIN_NOISE_OCTAVES = 5;
static const utf8_char VIEW_EFFECTS_CELL = UTF8("█");
static const enum Color VIEW_MAX_OUTSIZE_COLOR = BRIGHT_CYAN;
#endif

struct View {
    VectorU position;
    struct ncplane *plane;
    struct ncplane_options plane_options;
#ifdef COOL_EFFECTS
    float perlin_seed;
    float shift;
#endif
};

void init_view(View *view,
               VectorU initial_position,
               VectorU size,
               VectorU parent_size,
               enum Color background_color,
               struct ncplane *parent_plane);
void draw_world_on_view(View *view, World *world);

static inline void free_view(View *view) {
    ncplane_destroy(view->plane);
    view->plane = nullptr;
}
