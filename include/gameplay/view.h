#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/vector.h"

struct View {
    Vector position;
    struct ncplane *plane;
    struct ncplane_options plane_options;
};

void init_view(View *view, Vector initial_position, Vector size, Vector parent_size, struct ncplane *parent_plane);
void draw_world_on_view(View *view, World *world);

void update_view_position(View *view, World *world, Vector position);

static inline void free_view(View *view) { ncplane_destroy(view->plane); }
