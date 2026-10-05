#pragma once

#include "constants.h"
#include "interface/button.h"
#include "interface/text.h"
#include "notcurses/notcurses.h"

#define DEFINE_INTERFACE(NAME, TEXT_COUNT, BUTTON_COUNT) \
    typedef struct {                                     \
        Color background_color;                          \
        int selected_button;                             \
        int key_held_cooldown;                           \
        void (*escape_handler)(void *);                  \
        void *escape_handler_arg;                        \
        Text texts[TEXT_COUNT];                          \
        Button buttons[BUTTON_COUNT];                    \
    } NAME;

#define DEFINE_BUTTONLESS_INTERFACE(NAME, TEXT_COUNT) \
    typedef struct {                                  \
        Color background_color;                       \
        int selected_button;                          \
        int key_held_cooldown;                        \
        void (*escape_handler)(void *);               \
        void *escape_handler_arg;                     \
        Text texts[TEXT_COUNT];                       \
    } NAME;

static const int KEY_HELD_COOLDOWN = KEY_HELD_COOLDOWN_TIME_MS / (1000 / UPDATE_FREQUENCY);

// FIX: BG colors aren't working that well
#define DEFINE_INIT_INTERFACE(NAME, FUNCTION_NAME)                                          \
    static inline void init_##FUNCTION_NAME##_interface(NAME *interface,                    \
                                                        Color background_color,             \
                                                        void *escape_handler_arg,           \
                                                        size_t escape_handler_arg_size,     \
                                                        void (*escape_handler)(void *)) {   \
        interface->background_color = HYPER_DARK_GRAY;                                      \
                                                                                            \
        interface->selected_button = 0;                                                     \
        interface->key_held_cooldown = KEY_HELD_COOLDOWN;                                   \
                                                                                            \
        interface->escape_handler = escape_handler;                                         \
                                                                                            \
        interface->escape_handler_arg = malloc(escape_handler_arg_size);                    \
        memcpy(interface->escape_handler_arg, escape_handler_arg, escape_handler_arg_size); \
                                                                                            \
        toggle_selection(&interface->buttons[0]);                                           \
    }

#define DEFINE_INIT_BUTTONLESS_INTERFACE(NAME, FUNCTION_NAME)                               \
    static inline void init_##FUNCTION_NAME##_interface(NAME *interface,                    \
                                                        Color background_color,             \
                                                        void *escape_handler_arg,           \
                                                        size_t escape_handler_arg_size,     \
                                                        void (*escape_handler)(void *)) {   \
        interface->background_color = HYPER_DARK_GRAY;                                      \
                                                                                            \
        interface->selected_button = -1;                                                    \
        interface->key_held_cooldown = KEY_HELD_COOLDOWN;                                   \
                                                                                            \
        interface->escape_handler = escape_handler;                                         \
                                                                                            \
        interface->escape_handler_arg = malloc(escape_handler_arg_size);                    \
        memcpy(interface->escape_handler_arg, escape_handler_arg, escape_handler_arg_size); \
    }

void handle_interface_input(int *selected_button,
                            int *key_held_cooldown,
                            Button *buttons,
                            unsigned int button_count,
                            void (*escape_handler)(void *),
                            void *escape_handler_arg,
                            int key);

void draw_interface(struct ncplane *plane,
                    Color background_color,
                    Text *texts,
                    unsigned int text_count,
                    Button *buttons,
                    unsigned int button_count);

void free_interface(Text *texts,
                    unsigned int text_count,
                    Button *buttons,
                    unsigned int button_count,
                    void **escape_handler_arg);
