#include "../../../include/platform/windows/windows_terminal.h"

#include <Windows.h>

#include "../include/core.h"
#include "../include/vector2D.h"

void init_terminal_context(TerminalContext *terminal_context, Vector2D size) {
    terminal_context->size = size;

    terminal_context->outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    terminal_context->rect.Top = 0;
    terminal_context->rect.Bottom = (short)(terminal_context->size.y - 1);
    terminal_context->rect.Left = 0;
    terminal_context->rect.Right = (short)(terminal_context->size.x - 1);

    COORD coord = {(short)consoleCtxPtr->size.x, (short)consoleCtxPtr->size.y};

    SetConsoleTitle("Survive");
    SetConsoleScreenBufferSize(terminal_context->outputHandle, coord);
    SetConsoleActiveScreenBuffer(terminal_context->outputHandle);
    SetConsoleWindowInfo(terminal_context->outputHandle, TRUE, &terminal_context->rect);

    terminal_context->buffer = malloc(terminal_context->size.x * terminal_context->size.y * sizeof(CHAR_INFO));
}

void free_terminal_context(TerminalContext *terminal_context) {
    CloseHandle(terminal_context->outputHandle);
    FreeConsole();
}

void put_char(TerminalContext *terminal_context, Vector2D position, char c, unsigned short color) {
    if (position.x >= 0 && position.x < terminal_context->size.x && position.y >= 0 &&
        position.y < terminal_context->size.y) {
        terminal_context->buffer[position.y * terminal_context->size.x + position.x].Attributes = color;
        terminal_context->buffer[position.y * terminal_context->size.x + position.x].Char.AsciiChar = c;
    }
}

void set_cursor(TerminalContext *terminal_context, Vector2D position) {
    COORD coord = {(SHORT)position.x, (SHORT)position.y};
    SetConsoleCursorPosition(terminal_context->outputHandle, coord);
}

/*  Coloca um string na posição (x, y) com a cor especificada (0 a 15).
 */
void print(TerminalContext *terminal_context, char *string, unsigned short color, Vector2D position) {
    Vector2D calculated = position;
    int i = 0;
    char c;

    while (1) {
        c = string[i];

        if (c) {
            if (c != '\n') {
                calculated.x++;
                SetCharOnPosition(terminal_context, calculated, string[i], color);
            } else {
                calculated.x = position.x;
                calculated.y++;
            }

            i++;
        } else {
            break;
        }
    }
}

void write_terminal(TerminalContext *terminal_context) {
    COORD size = {(SHORT)terminal_context->size.x, (SHORT)terminal_context->size.y};
    COORD coord = {0, 0};

    WriteConsoleOutputA(terminal_context->outputHandle, terminal_context->buffer, size, coord, &terminal_context->rect);
}

void clear_terminal(TerminalContext *terminal_context) {
    for (int i = 0; i < terminal_context->size.y; i++) {
        for (int j = 0; j < terminal_context->size.x; j++) {
            SetCharOnPosition(terminal_context, CreateVector2D(j, i), 32, 0x00);
        }
    }

    WriteOutput(terminal_context);
}
