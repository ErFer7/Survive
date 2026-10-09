#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

#define COOL_EFFECTS

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

static inline void update_view_position(View *view, VectorU position) { view->position = position; }

static inline void free_view(View *view) {
    ncplane_destroy(view->plane);
    view->plane = nullptr;
}
