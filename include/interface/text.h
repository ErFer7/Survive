#pragma once

#include <notcurses/notcurses.h>

#include "../utils/vector.h"
#include "alignment.h"

typedef struct {
    char *content;
    char **lines;
    size_t length;
    Vector size;
    unsigned int foreground_color;
    unsigned int background_color;
    Vector position;
    Vector aligned_position;
    enum Alignment alignment;
} Text;

void init_text(Text *text,
               const char *content,
               size_t length,
               unsigned int foreground_color,
               unsigned int background_color,
               Vector position,
               enum Alignment alignment,
               Vector parent_size);
void draw_text(Text *text, struct ncplane *plane);
unsigned int get_background_color(Text *text);
void set_background_color(Text *text, unsigned int background_color);
unsigned int get_foreground_color(Text *text);
void set_foreground_color(Text *text, unsigned int foreground_color);
void free_text(Text *text);
