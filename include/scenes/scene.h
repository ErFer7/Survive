#pragma once

#include <notcurses/notcurses.h>

typedef struct _Interface {
    void (*enter)(void *);
    void (*update)(void *);
    void (*exit)(void *);
    struct ncplane_options plane_options;
    struct ncplane *plane;
} Scene;
