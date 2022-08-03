#pragma once

#include <Windows.h>

#define ANIMATION_SPEED 16.0f

extern unsigned int consoleWidth, consoleHeight;
extern HANDLE consoleOutputHandle;
extern SMALL_RECT consoleRect;
extern CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(unsigned int width, unsigned int height);
void FreeConsoleRenderer();
void SetCharOnPosition(int x, int y, char c, unsigned short color);
void PrintStringOnPosition(char *s, unsigned short color, int x, int y);
void SetCursorPosition(int x, int y);
void WriteOutput();
void ClearOutput();
