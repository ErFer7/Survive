#include "interface/interface.h"

#include "interface/button.h"
#include "notcurses/nckeys.h"
#include "notcurses/notcurses.h"

void handle_interface_input(int *selected_button,
                            int *key_held_cooldown,
                            Button *buttons,
                            unsigned int button_count,
                            void (*escape_handler)(void *),
                            void **escape_handler_arg,
                            struct notcurses *not_curses) {
    unsigned int key = notcurses_get_nblock(not_curses, nullptr);

    if (*key_held_cooldown > 0) {
        (*key_held_cooldown)--;

        return;
    }

    switch (key) {
        case NCKEY_ESC:
            escape_handler(*escape_handler_arg);
            *key_held_cooldown = KEY_HELD_COOLDOWN;
            break;
        case NCKEY_ENTER:
            trigger(&buttons[*selected_button]);
            *key_held_cooldown = KEY_HELD_COOLDOWN;
            break;
        case NCKEY_UP:
            if (*selected_button > 0) {
                toggle_selection(&buttons[*selected_button]);
                (*selected_button)--;
                toggle_selection(&buttons[*selected_button]);
            }
            *key_held_cooldown = KEY_HELD_COOLDOWN;

            break;
        case NCKEY_DOWN:
            if (*selected_button < button_count - 1) {
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

void draw_interface(struct ncplane *plane,
                    Color background_color,
                    Text *texts,
                    unsigned int text_count,
                    Button *buttons,
                    unsigned int button_count) {
    ncplane_set_bg_rgb(plane, background_color);

    for (unsigned int i = 0; i < text_count; i++) {
        draw_text(&texts[i], plane);
    }

    for (unsigned int i = 0; i < button_count; i++) {
        draw_button(&buttons[i], plane);
    }

    ncplane_set_bg_default(plane);
}

void free_interface(Text *texts,
                    unsigned int text_count,
                    Button *buttons,
                    unsigned int button_count,
                    void **escape_handler_arg) {
    for (unsigned int i = 0; i < text_count; i++) {
        free_text(&texts[i]);
    }

    for (unsigned int i = 0; i < button_count; i++) {
        free_button(&buttons[i]);
    }

    free(escape_handler_arg);
}
