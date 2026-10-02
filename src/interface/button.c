#include "interface/button.h"

void init_button(Button *button,
                 const char *content,
                 size_t length,
                 unsigned int foreground_color,
                 unsigned int background_color,
                 Vector position,
                 enum Alignment alignment,
                 Vector parent_size,
                 const CallArguments *call_arguments,
                 size_t call_arguments_size,
                 void (*call)(CallArguments *)) {
    init_text(&button->text, content, length, foreground_color, background_color, position, alignment, parent_size);

    button->call_arguments = malloc(call_arguments_size);
    memcpy(button->call_arguments, call_arguments, call_arguments_size);

    button->call = call;
}

void draw_button(Button *button, struct ncplane *plane) { draw_text(&button->text, plane); }

void toggle_selection(Button *button) {
    unsigned int new_background_color = get_foreground_color(&button->text);
    unsigned int new_foreground_color = get_background_color(&button->text);

    set_foreground_color(&button->text, new_foreground_color);
    set_background_color(&button->text, new_background_color);
}

void trigger(Button *button) {
    if (button->call != nullptr) {
        button->call(button->call_arguments);
    }
}

void free_button(Button *button) {
    free_text(&button->text);
    free(button->call_arguments);
}
