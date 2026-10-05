#include "interface/text.h"

#include <notcurses/notcurses.h>

void init_text(Text *text,
               const char *content,
               size_t length,
               unsigned int foreground_color,
               Color background_color,
               Vector position,
               enum Alignment alignment,
               Vector parent_size) {
    text->content = malloc(sizeof(char) * (length + 1));
    memcpy(text->content, content, sizeof(char) * (length + 1));

    text->length = length;
    text->foreground_color = foreground_color;
    text->background_color = background_color;
    text->position = position;
    text->alignment = alignment;

    text->lines = malloc(sizeof(char *));
    text->lines[0] = text->content;

    char *line = text->content;
    int width = 0;
    int height = 1;

    while (1) {
        char *new_line = strchr(line, '\n');

        if (new_line != nullptr) {
            text->lines = realloc(text->lines, sizeof(char *) * (height + 1));
            text->lines[height] = new_line + 1;
            height++;

            *new_line = '\0';
        }

        int line_width = ncstrwidth(line, nullptr, nullptr);

        if (line_width > width) {
            width = line_width;
        }

        if (new_line != nullptr) {
            line = new_line + 1;
        } else {
            break;
        }
    }

    text->size.x = width;
    text->size.y = height;

    text->aligned_position = aligned_position(text->position, text->size, parent_size, alignment);
}

void draw_text(Text *text, struct ncplane *plane) {
    ncplane_set_fg_rgb(plane, text->foreground_color);
    ncplane_set_bg_rgb(plane, text->background_color);

    unsigned int line_index = 0;
    for (unsigned int i = text->aligned_position.y; i < text->aligned_position.y + text->size.y; i++) {
        ncplane_putstr_yx(plane, i, text->aligned_position.x, text->lines[line_index]);
        line_index++;
    }

    ncplane_set_fg_default(plane);
    ncplane_set_bg_default(plane);
}

void free_text(Text *text) {
    if (text->content != nullptr) {
        free(text->content);
        text->content = nullptr;
    }

    if (text->lines != nullptr) {
        free(text->lines);
        text->lines = nullptr;
    }
}
