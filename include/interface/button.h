#pragma once

#include "text.h"

typedef struct _Interface CallArguments;

typedef struct {
    Text text;
    CallArguments *call_arguments;
    void (*call)(CallArguments *);
} Button;

void init_button(Button *button,
                 const char *content,
                 size_t length,
                 unsigned int foreground_color,
                 unsigned int background_color,
                 Vector position,
                 enum Alignment alignment,
                 Vector parent_size,
                 const CallArguments *call_arguments,
                 size_t call_arguments_size,
                 void (*call)(CallArguments *));
void draw_button(Button *button, struct ncplane *plane);
void toggle_selection(Button *button);
void trigger(Button *button);
void free_button(Button *button);
