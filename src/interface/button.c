#include "interface/button.h"

void init_button(Button *button,
                 const char *content,
                 size_t length,
                 enum Color foreground_color,
                 enum Color background_color,
                 Vector position,
                 enum Alignment alignment,
                 VectorU parent_size,
                 void *call_arg,
                 void (*call)(void *)) {
    init_text(&button->text, content, length, foreground_color, background_color, position, alignment, parent_size);

    button->call_arg = call_arg;
    button->call = call;
}

void toggle_selection(Button *button) {
    enum Color new_background_color = button->text.foreground_color;
    enum Color new_foreground_color = button->text.background_color;

    button->text.foreground_color = new_foreground_color;
    button->text.background_color = new_background_color;
}

void free_button(Button *button) {
    free_text(&button->text);

    button->call_arg = nullptr;
}
