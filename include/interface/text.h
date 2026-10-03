#pragma once

#include <notcurses/notcurses.h>

#include "alignment.h"
#include "utils/colors.h"

struct Text {
    char *content;
    char **lines;
    size_t length;
    Vector size;
    Color foreground_color;
    Color background_color;
    Vector position;
    Vector aligned_position;
    enum Alignment alignment;
};

void init_text(Text *text,
               const char *content,
               size_t length,
               unsigned int foreground_color,
               Color background_color,
               Vector position,
               enum Alignment alignment,
               Vector parent_size);
void draw_text(Text *text, struct ncplane *plane);

static inline Color get_background_color(Text *text) { return text->background_color; }

static inline void set_background_color(Text *text, Color background_color) {
    text->background_color = background_color;
}

static inline Color get_foreground_color(Text *text) { return text->foreground_color; }

static inline void set_foreground_color(Text *text, Color foreground_color) {
    text->foreground_color = foreground_color;
}

void free_text(Text *text);
