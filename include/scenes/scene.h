#pragma once

#include <notcurses/notcurses.h>

#include "types.h"

struct Scene {
    void (*enter)(void *, void *);
    void (*update)(void *);
    void (*draw)(void *);
    void (*exit)(void *);
    struct ncplane_options plane_options;
    struct ncplane *plane;
};

void init_scene(Scene *scene,
                void (*enter)(void *, void *),
                void (*update)(void *),
                void (*draw)(void *),
                void (*exit)(void *),
                Vector parent_size,
                struct ncplane *parent_plane);
void free_scene(Scene *scene);
