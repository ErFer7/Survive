#pragma once

#include <stdint.h>
#include <Windows.h>

#define ANIMATION_SPEED 16.0f

unsigned int consoleWidth, consoleHeight;
HANDLE consoleOutputHandle;
SMALL_RECT consoleRect;
SMALL_RECT windowRect;
CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(unsigned int width, unsigned int height);
void FreeConsoleRenderer();
void SetCharOnPosition(unsigned int x, unsigned int y, char c, unsigned short color);
void PrintStringOnPosition(char *s, uint8_t color, unsigned int x, unsigned int y);
void SetCursorPosition(unsigned int x, unsigned int y);
void WriteOutput();
void ClearOutput();
