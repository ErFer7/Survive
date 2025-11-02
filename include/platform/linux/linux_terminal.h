#pragma once

#include "../../core.h"
#include "../../vector2D.h"

typedef struct {
    Vector2D size;
    int pressed_keys[2];
} TerminalContext;

typedef struct {
    EventStateContext *event_state_context;
    TerminalContext *terminal_context;
} InputPollingThreadArg;

void init_terminal_context(TerminalContext *terminal_context, Vector2D size);
void free_terminal_context(TerminalContext *terminal_context);
void put_char(TerminalContext *terminal_context, Vector2D position, char c, unsigned short color);
void set_cursor(TerminalContext *terminal_context, Vector2D position);
void print(TerminalContext *terminal_context, char *string, unsigned short color, Vector2D position);
void write(TerminalContext *terminal_context);
void clear(TerminalContext *terminal_context);
void poll_input(void *input_polling_thread_arg);
int is_key_pressed(TerminalContext *terminal_context, int key);
