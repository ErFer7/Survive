#include "../include/graphics.h"

#include <Windows.h>

#include "../include/vector2D.h"
#include "../include/core.h"

void InitConsoleContext(ConsoleContext *consoleCtxPtr, Vector2D size)
{
    consoleCtxPtr->size = size;

    consoleCtxPtr->outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    consoleCtxPtr->rect.Top = 0;
    consoleCtxPtr->rect.Bottom = (short)(consoleCtxPtr->size.y - 1);
    consoleCtxPtr->rect.Left = 0;
    consoleCtxPtr->rect.Right = (short)(consoleCtxPtr->size.x - 1);

    SetConsoleWindowInfo(consoleCtxPtr->outputHandle, TRUE, &consoleCtxPtr->rect);

    COORD coord = {(short)consoleCtxPtr->size.x, (short)consoleCtxPtr->size.y};

    SetConsoleScreenBufferSize(consoleCtxPtr->outputHandle, coord);
    SetConsoleActiveScreenBuffer(consoleCtxPtr->outputHandle);


    consoleCtxPtr->buffer = malloc(consoleCtxPtr->size.x * consoleCtxPtr->size.y * sizeof(CHAR_INFO));
}

void FreeConsoleContext(ConsoleContext *consoleCtxPtr)
{
    CloseHandle(consoleCtxPtr->outputHandle);
    FreeConsole();
}

void SetCharOnPosition(ConsoleContext *consoleCtxPtr, Vector2D position, char c, unsigned short color)
{
    if (position.x >= 0 && position.x < consoleCtxPtr->size.x && position.y >= 0 && position.y < consoleCtxPtr->size.y)
    {
        consoleCtxPtr->buffer[position.y * consoleCtxPtr->size.x + position.x].Attributes = color;
        consoleCtxPtr->buffer[position.y * consoleCtxPtr->size.x + position.x].Char.AsciiChar = c;
    }
}

void SetCursorPosition(ConsoleContext *consoleCtxPtr, Vector2D position)
{
    COORD coord = {(SHORT)position.x, (SHORT)position.y};
    SetConsoleCursorPosition(consoleCtxPtr->outputHandle, coord);
}

/*  Coloca um string na posição (x, y) com a cor especificada (0 a 15).
*/
void PrintStringOnPosition(ConsoleContext *consoleCtxPtr, char *string, unsigned short color, Vector2D position)
{
    Vector2D calculated = position;
    int i = 0;
    char c;

    while (1)
    {
        c = string[i];

        if (c != '\0')
        {
            if (c != '\n')
            {
                calculated.x++;
                SetCharOnPosition(consoleCtxPtr, calculated, string[i], color);
            }
            else
            {
                calculated.x = position.x;
                calculated.y++;
            }

            i++;
        }
        else
        {
            break;
        }
    }
}

void WriteOutput(ConsoleContext *consoleCtxPtr)
{
    COORD size = {(SHORT)consoleCtxPtr->size.x, (SHORT)consoleCtxPtr->size.y};
    COORD coord = {0, 0};

    WriteConsoleOutputA(consoleCtxPtr->outputHandle, consoleCtxPtr->buffer, size, coord, &consoleCtxPtr->rect);
}

void ClearOutput(ConsoleContext *consoleCtxPtr)
{
    for (int i = 0; i < consoleCtxPtr->size.y; i++)
    {
        for (int j = 0; j < consoleCtxPtr->size.x; j++)
        {
            SetCharOnPosition(consoleCtxPtr, CreateVector2D(j, i), 32, 0x00);
        }
    }

    WriteOutput(consoleCtxPtr);
}
