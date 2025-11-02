#include "../../../include/platform/linux/linux_terminal.h"

#include <ncurses.h>

// Size is only defined here because of compatibility reasons
void init_terminal_context(TerminalContext *terminal_context, Vector2D size) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    start_color();
    curs_set(0);
    nodelay(stdscr, TRUE);

    getmaxyx(stdscr, terminal_context->size.x, terminal_context->size.y);

    printf("\033]0;%s\007", "Survive");  // TODO: Check if this works
}

void free_terminal_context(TerminalContext *terminal_context) { endwin(); }

void put_char(TerminalContext *console_context, Vector2D position, char c, unsigned short color) {
    if (position.x >= 0 && position.x < console_context->size.x && position.y >= 0 &&
        position.y < console_context->size.y) {
        mvaddch(position.y, position.x, c | COLOR_PAIR(color));
    }
}

void set_cursor(TerminalContext *terminal_context, Vector2D position) { move(position.y, position.x); }

void print(TerminalContext *console_context, char *string, unsigned short color, Vector2D position) {
    Vector2D current = position;
    int i = 0;
    char c;

    attron(COLOR_PAIR(color));

    while ((c = string[i++])) {
        if (c != '\n') {
            // TODO: Optimize this comparison
            if (current.x >= 0 && current.x < console_context->size.x && current.y >= 0 &&
                current.y < console_context->size.y) {
                mvaddch(current.y, current.x, c);
            }
            current.x++;
        } else {
            current.y++;
            current.x = position.x;
        }
    }

    attroff(COLOR_PAIR(color));
}

void write_terminal(TerminalContext *terminal_context) { refresh(); }

void clear_terminal(TerminalContext *terminal_context) { clear(); }

void poll_input(void *input_polling_thread_arg) {
    EventStateContext *event_state_context = ((InputPollingThreadArg *)input_polling_thread_arg)->event_state_context;
    TerminalContext *terminal_context = ((InputPollingThreadArg *)input_polling_thread_arg)->terminal_context;

    // TODO: Refactor this
    while (event_state_context->state != EXIT) {
        terminal_context->pressed_keys[0] = getch();
        terminal_context->pressed_keys[1] = getch();
    }
}

int is_key_pressed(TerminalContext *terminal_context, int key) {
    return terminal_context->pressed_keys[0] == key || terminal_context->pressed_keys[1] == key;
}
