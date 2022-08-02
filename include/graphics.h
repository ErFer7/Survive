#pragma once

#include <Windows.h>

#define ANIMATION_SPEED 16.0f

extern unsigned int consoleWidth, consoleHeight;
extern HANDLE consoleOutputHandle;
extern SMALL_RECT consoleRect;
extern CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(unsigned int width, unsigned int height);
void FreeConsoleRenderer();
void SetCharOnPosition(unsigned int x, unsigned int y, char c, unsigned short color);
void PrintStringOnPosition(char *s, unsigned int color, unsigned int x, unsigned int y);
void SetCursorPosition(unsigned int x, unsigned int y);
void WriteOutput();
void ClearOutput();
