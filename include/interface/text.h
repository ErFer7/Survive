#pragma once

#include <notcurses/notcurses.h>

#include "alignment.h"
#include "utils/color.h"

struct Text {
    char *content;
    char **lines;
    size_t length;
    VectorU size;
    enum Color foreground_color;
    enum Color background_color;
    Vector position;
    VectorU aligned_position;
    enum Alignment alignment;
};

void init_text(Text *text,
               const char *content,
               size_t length,
               enum Color foreground_color,
               enum Color background_color,
               Vector position,
               enum Alignment alignment,
               VectorU parent_size);
void draw_text(Text *text, struct ncplane *plane);

static inline enum Color get_background_color(Text *text) { return text->background_color; }

static inline void set_background_color(Text *text, enum Color background_color) {
    text->background_color = background_color;
}

static inline enum Color get_foreground_color(Text *text) { return text->foreground_color; }

static inline void set_foreground_color(Text *text, enum Color foreground_color) {
    text->foreground_color = foreground_color;
}

static inline void set_single_line_text_content(Text *text, char *new_content, size_t length) {
    memcpy(text->content, new_content, length);
}

void free_text(Text *text);
