#pragma once

#include "../../vector2D.h"

typedef struct {
    Vector2D size;
} TerminalContext;

void init_terminal_context(TerminalContext *terminal_context, Vector2D size);
void free_terminal_context(TerminalContext *terminal_context);
void put_char(TerminalContext *terminal_context, Vector2D position, char c, unsigned short color);
void set_cursor(TerminalContext *terminal_context, Vector2D position);
void print(TerminalContext *terminal_context, char *string, unsigned short color, Vector2D position);
void write(TerminalContext *terminal_context);
void clear(TerminalContext *terminal_context);
