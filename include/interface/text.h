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
void free_text(Text *text);

static inline void set_single_line_text_content(Text *text, char *new_content, size_t length) {
    memcpy(text->content, new_content, length);
}
