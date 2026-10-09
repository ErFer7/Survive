#include "interface/interface.h"

#include <notcurses/notcurses.h>

#include "interface/button.h"
#include "system/input.h"

void handle_interface_input(int32_t *selected_button,
                            int32_t *key_held_cooldown,
                            Button *buttons,
                            unsigned int button_count,
                            void (*escape_handler)(void *),
                            void *escape_handler_arg,
                            InputState *input_state) {
    if (*key_held_cooldown > 0) {
        (*key_held_cooldown)--;

        return;
    }

    if (*selected_button == -1) {
        if (is_key_pressed(input_state, KEY_ESC)) {
            escape_handler(escape_handler_arg);
            *key_held_cooldown = KEY_HELD_COOLDOWN;
        }

        return;
    }

    // PERFORMANCE: Simplify this check
    switch (get_key_pressed(input_state)) {
        case KEY_ESC:
            escape_handler(escape_handler_arg);
            *key_held_cooldown = KEY_HELD_COOLDOWN;
            break;
        case KEY_ENTER:
            trigger(&buttons[*selected_button]);
            *key_held_cooldown = KEY_HELD_COOLDOWN;
            break;
        case KEY_UP:
            if (*selected_button > 0) {
                toggle_selection(&buttons[*selected_button]);
                (*selected_button)--;
                toggle_selection(&buttons[*selected_button]);
            }
            *key_held_cooldown = KEY_HELD_COOLDOWN;

            break;
        case KEY_DOWN:
            if ((uint32_t)*selected_button < button_count - 1) {
                toggle_selection(&buttons[*selected_button]);
                (*selected_button)++;
                toggle_selection(&buttons[*selected_button]);
            }
            *key_held_cooldown = KEY_HELD_COOLDOWN;

            break;
        default:
            break;
    }
}

void draw_interface(struct ncplane *plane, Text *texts, uint32_t text_count, Button *buttons, uint32_t button_count) {
    for (uint32_t i = 0; i < text_count; i++) {
        draw_text(&texts[i], plane);
    }

    for (uint32_t i = 0; i < button_count; i++) {
        draw_button(&buttons[i], plane);
    }
}

void free_interface(Text *texts,
                    uint32_t text_count,
                    Button *buttons,
                    uint32_t button_count,
                    void **escape_handler_arg) {
    for (uint32_t i = 0; i < text_count; i++) {
        free_text(&texts[i]);
    }

    for (uint32_t i = 0; i < button_count; i++) {
        free_button(&buttons[i]);
    }

    if (escape_handler_arg != nullptr) {
        free(escape_handler_arg);
        escape_handler_arg = nullptr;
    }
}
