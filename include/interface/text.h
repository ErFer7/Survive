#pragma once

#include <notcurses/notcurses.h>

#include "../utils/vector.h"
#include "alignment.h"

typedef struct {
    char *content;
    size_t length;
    Vector size;
    unsigned int color;
    Vector position;
    Vector aligned_position;
    enum Alignment alignment;
} Text;

void init_text(Text *text,
               const char *content,
               size_t length,
               unsigned int color,
               Vector position,
               enum Alignment alignment,
               Vector parent_size);
void draw_text(Text *text, struct ncplane *plane);
void free_text(Text *text);
