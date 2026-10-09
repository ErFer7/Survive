#pragma once

#include <notcurses/notcurses.h>

#include "types.h"

#define KEY_COUNT 9  // C requires the array initialization to not be a static const for some reason

enum Key { NONE, KEY_ESC, KEY_ENTER, KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_SHIFT, KEY_CTRL };

struct InputState {
    bool is_key_pressed[KEY_COUNT];
};

void init_input_state(InputState *input_state);

void update_input_state(InputState *input_state, struct notcurses *not_curses);

static inline bool is_key_pressed(InputState *input_state, enum Key key) { return input_state->is_key_pressed[key]; }

// First key pressed in the state array
static inline enum Key get_key_pressed(InputState *input_state) {
    for (enum Key key = KEY_ESC; key < KEY_CTRL; key++) {
        if (input_state->is_key_pressed[key]) {
            return key;
        }
    }

    return NONE;
}
