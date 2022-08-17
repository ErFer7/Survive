#pragma once

#include <Windows.h>

#include "../include/vector2D.h"

typedef struct
{
    Vector2D size;
    HANDLE outputHandle;
    SMALL_RECT rect;
    CHAR_INFO *buffer;
} ConsoleContext;

void InitConsoleContext(ConsoleContext *consoleCtxPtr, Vector2D size);
void FreeConsoleContext(ConsoleContext *consoleCtxPtr);
void SetCharOnPosition(ConsoleContext *consoleCtxPtr, Vector2D position, char c, unsigned short color);
void SetCursorPosition(ConsoleContext *consoleCtxPtr, Vector2D position);
void PrintStringOnPosition(ConsoleContext *consoleCtxPtr, char *string, unsigned short color, Vector2D position);
void WriteOutput(ConsoleContext *consoleCtxPtr);
void ClearOutput(ConsoleContext *consoleCtxPtr);
