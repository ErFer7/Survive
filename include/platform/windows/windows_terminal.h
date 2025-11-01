#pragma once

#include <Windows.h>
#include <uchar.h>

#include "../include/vector2D.h"

typedef struct {
    Vector2D size;
    HANDLE outputHandle;
    SMALL_RECT rect;
    CHAR_INFO *buffer;
} TerminalContext;

void init_terminal_context(TerminalContext *terminal_context, Vector2D size);
void free_terminal_context(TerminalContext *terminal_context);
void put_char(TerminalContext *terminal_context, Vector2D position, char c, unsigned short color);
void set_cursor(TerminalContext *terminal_context, Vector2D position);
void print(TerminalContext *terminal_context, char *string, unsigned short color, Vector2D position);
void write_terminal(TerminalContext *terminal_context);
void clear_terminal(TerminalContext *terminal_context);
