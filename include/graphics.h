#pragma once

#include <Windows.h>

extern int consoleWidth, consoleHeight;
extern HANDLE consoleOutputHandle;
extern SMALL_RECT consoleRect;
extern CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(int consoleWidth_, int consoleHeight_);
void FreeConsoleRenderer();
void SetCharOnPosition(int x, int y, char c, unsigned short color);
void PrintStringOnPosition(char *s, unsigned short color, int x, int y);
void SetCursorPosition(int x, int y);
void WriteOutput();
void ClearOutput();
