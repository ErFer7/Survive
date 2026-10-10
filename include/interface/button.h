#pragma once

#include "interface/text.h"

struct Button {
    Text text;
    void *call_arg;
    void (*call)(void *);
};

void init_button(Button *button,
                 const char *content,
                 size_t length,
                 enum Color foreground_color,
                 enum Color background_color,
                 Vector position,
                 enum Alignment alignment,
                 VectorU parent_size,
                 void *call_arg,
                 size_t call_arg_size,
                 void (*call)(void *));
void toggle_selection(Button *button);
void free_button(Button *button);

static inline void draw_button(Button *button, struct ncplane *plane) { draw_text(&button->text, plane); }

static inline void trigger(Button *button) { button->call(button->call_arg); }
