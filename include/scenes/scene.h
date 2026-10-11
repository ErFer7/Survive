#pragma once

#include <notcurses/notcurses.h>

#include "types.h"
#include "utils/color.h"
#include "utils/vector.h"

struct Scene {
    void (*enter)(void *, void *);
    void (*update)(void *);
    void (*draw)(void *);
    void (*exit)(void *, void *);
    struct ncplane_options plane_options;
    struct ncplane *plane;
    struct ncplane *parent_plane;
};

void init_scene(Scene *scene,
                void *derived_scene,
                void (*enter)(void *, void *),
                void (*update)(void *),
                void (*draw)(void *),
                void (*exit)(void *, void *),
                int (*resize_callback)(struct ncplane *),
                VectorU parent_size,
                enum Color background_color,
                struct ncplane *parent_plane);
void free_scene(Scene *scene);

static inline void resize_scene(Scene *scene, VectorU size) { ncplane_resize_simple(scene->plane, size.y, size.x); }
