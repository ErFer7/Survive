#include "interface/button.h"

void init_button(Button *button,
                 const char *content,
                 size_t length,
                 unsigned int foreground_color,
                 unsigned int background_color,
                 Vector position,
                 enum Alignment alignment,
                 Vector parent_size,
                 void *call_arg,
                 size_t call_arg_size,
                 void (*call)(void *)) {
    init_text(&button->text, content, length, foreground_color, background_color, position, alignment, parent_size);

    if (call_arg_size > 0) {
        button->call_arg = malloc(call_arg_size);
        memcpy(button->call_arg, call_arg, call_arg_size);
    } else {
        button->call_arg = nullptr;
    }

    button->call = call;
}

void toggle_selection(Button *button) {
    unsigned int new_background_color = get_foreground_color(&button->text);
    unsigned int new_foreground_color = get_background_color(&button->text);

    set_foreground_color(&button->text, new_foreground_color);
    set_background_color(&button->text, new_background_color);
}

void free_button(Button *button) {
    free_text(&button->text);

    if (button->call_arg != nullptr) {
        free(button->call_arg);
        button->call_arg = nullptr;
    }
}
