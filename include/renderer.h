#pragma once

#include <Windows.h>

unsigned int consoleWidth, consoleHeight;
HANDLE consoleInputHandle, consoleOutputHandle;
SMALL_RECT consoleRect;
SMALL_RECT windowRect;
CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(unsigned int width, unsigned int height);
void FreeConsoleRenderer();
void SetCharOnPosition(unsigned int x, unsigned int y, char c, unsigned short color);
void WriteOutput();
