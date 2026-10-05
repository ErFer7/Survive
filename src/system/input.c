#include "system/input.h"

void init_input_state(InputState *input_state) {
    for (unsigned int i = 0; i < KEY_COUNT; i++) {
        input_state->is_key_pressed[i] = false;
    }
}

void update_input_state(InputState *input_state, struct notcurses *not_curses) {
    ncinput ni;
    unsigned int key = notcurses_get_nblock(not_curses, &ni);
    bool is_key_pressed = ni.evtype == NCTYPE_PRESS || ni.evtype == NCTYPE_REPEAT;

    switch (key) {
        case NCKEY_ESC:
            input_state->is_key_pressed[KEY_ESC] = is_key_pressed;
            break;
        case NCKEY_ENTER:
            input_state->is_key_pressed[KEY_ENTER] = is_key_pressed;
            break;
        case NCKEY_UP:
            input_state->is_key_pressed[KEY_UP] = is_key_pressed;
            break;
        case NCKEY_DOWN:
            input_state->is_key_pressed[KEY_DOWN] = is_key_pressed;
            break;
        case NCKEY_RIGHT:
            input_state->is_key_pressed[KEY_RIGHT] = is_key_pressed;
            break;
        case NCKEY_LEFT:
            input_state->is_key_pressed[KEY_LEFT] = is_key_pressed;
            break;
        default:
            break;
    }
}
