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
                 unsigned int foreground_color,
                 unsigned int background_color,
                 Vector position,
                 enum Alignment alignment,
                 Vector parent_size,
                 void *call_arg,
                 size_t call_arg_size,
                 void (*call)(void *));

static inline void draw_button(Button *button, struct ncplane *plane) { draw_text(&button->text, plane); }

void toggle_selection(Button *button);

static inline void trigger(Button *button) { button->call(button->call_arg); }

void free_button(Button *button);
