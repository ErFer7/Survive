#include "../include/graphics.h"

#include <Windows.h>

int consoleWidth, consoleHeight;
HANDLE consoleOutputHandle;
SMALL_RECT consoleRect;
CHAR_INFO *consoleBuffer;

void InitConsoleRenderer(int width, int height)
{
    consoleWidth = width > 255 ? 255 : width;
    consoleHeight = height > 255 ? 255 : height;

    consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    consoleRect.Top = 0;
    consoleRect.Bottom = (short)(consoleHeight - 1);
    consoleRect.Left = 0;
    consoleRect.Right = (short)(consoleWidth - 1);

    SetConsoleWindowInfo(consoleOutputHandle, TRUE, &consoleRect);

    COORD coord = {(short)consoleWidth, (short)consoleHeight};

    SetConsoleScreenBufferSize(consoleOutputHandle, coord);
    SetConsoleActiveScreenBuffer(consoleOutputHandle);

    consoleBuffer = malloc(consoleWidth * consoleHeight * sizeof(CHAR_INFO));   // Aloca a memória para o buffer do console
    memset(consoleBuffer, 0, consoleWidth * consoleHeight * sizeof(CHAR_INFO)); // Reseta o buffer
}

void FreeConsoleRenderer()
{
    CloseHandle(consoleOutputHandle);
    FreeConsole();
}

void SetCharOnPosition(int x, int y, char c, unsigned short color)
{
    if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight)
    {
        consoleBuffer[y * consoleWidth + x].Attributes = color;
        consoleBuffer[y * consoleWidth + x].Char.AsciiChar = c;
    }
}

void SetCursorPosition(int x, int y)
{
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(consoleOutputHandle, coord);
}

void PrintStringOnPosition(char *s, unsigned short color, int x, int y)
{
    /* Coloca um string na posição (x, y) com a cor especificada (0 a 15).
     */

    int calculatedX = x;
    int calculatedY = y;

    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == '\n') // Aumenta a altura quando uma nova linha é encontrada
        {
            calculatedX = x;
            calculatedY++;
        }
        else if (s[i] != '\0')
        {
            SetCharOnPosition(calculatedX++, calculatedY, s[i], color);
        }
    }
}

void WriteOutput()
{
    COORD bufferSize = {(SHORT)consoleWidth, (SHORT)consoleHeight};
    COORD bufferCoord = {0, 0};

    WriteConsoleOutputA(consoleOutputHandle, consoleBuffer, bufferSize, bufferCoord, &consoleRect);
}

void ClearOutput()
{
    for (int i = 0; i < consoleHeight; i++)
        for (int j = 0; j < consoleWidth; j++)
            SetCharOnPosition(j, i, 32, 0x00);

    WriteOutput();
}
