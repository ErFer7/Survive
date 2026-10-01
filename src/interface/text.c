#include "interface/text.h"

#include <notcurses/notcurses.h>

// FIX: We are reading trash at the end of the text
void init_text(Text *text,
               const char *content,
               size_t length,
               unsigned int color,
               Vector position,
               enum Alignment alignment,
               Vector parent_size) {
    text->content = malloc(sizeof(char) * length);
    memcpy(text->content, content, sizeof(char) * length);
    text->length = length;

    text->color = color;
    text->position = position;
    text->alignment = alignment;

    // TODO: Rework
    int max_width = 0;
    int height = 0;

    char *line_start = text->content;

    if (length > 0) {
        while (*line_start != '\0') {
            char *newline = strchr(line_start, '\n');
            height++;

            if (newline) {
                *newline = '\0';

                int line_width = ncstrwidth(line_start, nullptr, nullptr);
                if (line_width > max_width) {
                    max_width = line_width;
                }

                *newline = '\n';
                line_start = newline + 1;
            } else {
                int line_width = ncstrwidth(line_start, nullptr, nullptr);
                if (line_width > max_width) {
                    max_width = line_width;
                }
                break;
            }
        }
    }

    text->size.x = max_width;
    text->size.y = height;

    text->aligned_position = aligned_position(text->position, text->size, parent_size, alignment);
}

// TODO: Rework
void draw_text(Text *text, struct ncplane *plane) {
    ncplane_set_fg_rgb(plane, text->color);  // TODO: Reset

    int start_y = text->aligned_position.y;
    int start_x = text->aligned_position.x;
    int current_y = start_y;

    char *line_start = text->content;

    while (*line_start != '\0') {
        char *newline = strchr(line_start, '\n');

        if (newline) {
            *newline = '\0';
            ncplane_putstr_yx(plane, current_y, start_x, line_start);
            *newline = '\n';

            line_start = newline + 1;
            current_y++;
        } else {
            ncplane_putstr_yx(plane, current_y, start_x, line_start);
            break;
        }
    }
}

void free_text(Text *text) { free(text->content); }
