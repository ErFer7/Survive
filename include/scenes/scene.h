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
};

void init_scene(Scene *scene,
                void (*enter)(void *, void *),
                void (*update)(void *),
                void (*draw)(void *),
                void (*exit)(void *, void *),
                VectorU parent_size,
                enum Color background_color,
                struct ncplane *parent_plane);
void free_scene(Scene *scene);
