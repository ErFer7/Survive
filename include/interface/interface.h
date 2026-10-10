#pragma once

#include <notcurses/notcurses.h>

#include "config.h"
#include "interface/button.h"
#include "interface/text.h"

#define DEFINE_INTERFACE(NAME, TEXT_COUNT, BUTTON_COUNT) \
    typedef struct {                                     \
        int32_t selected_button;                         \
        int32_t key_held_cooldown;                       \
        void (*escape_handler)(void *);                  \
        void *escape_handler_arg;                        \
        Text texts[TEXT_COUNT];                          \
        Button buttons[BUTTON_COUNT];                    \
    } NAME;

#define DEFINE_BUTTONLESS_INTERFACE(NAME, TEXT_COUNT) \
    typedef struct {                                  \
        int32_t selected_button;                      \
        int32_t key_held_cooldown;                    \
        void (*escape_handler)(void *);               \
        void *escape_handler_arg;                     \
        Text texts[TEXT_COUNT];                       \
    } NAME;

#define DEFINE_INIT_INTERFACE(NAME, FUNCTION_NAME)                                          \
    static inline void init_##FUNCTION_NAME##_interface(NAME *interface,                    \
                                                        void *escape_handler_arg,           \
                                                        size_t escape_handler_arg_size,     \
                                                        void (*escape_handler)(void *)) {   \
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
                                                        void *escape_handler_arg,           \
                                                        size_t escape_handler_arg_size,     \
                                                        void (*escape_handler)(void *)) {   \
        interface->selected_button = -1;                                                    \
        interface->key_held_cooldown = KEY_HELD_COOLDOWN;                                   \
                                                                                            \
        interface->escape_handler = escape_handler;                                         \
                                                                                            \
        interface->escape_handler_arg = malloc(escape_handler_arg_size);                    \
        memcpy(interface->escape_handler_arg, escape_handler_arg, escape_handler_arg_size); \
    }

static const int32_t KEY_HELD_COOLDOWN_TIME_MS = 180;
static const int32_t KEY_HELD_COOLDOWN = KEY_HELD_COOLDOWN_TIME_MS / (1000 / UPDATE_FREQUENCY);

void handle_interface_input(int32_t *selected_button,
                            int32_t *key_held_cooldown,
                            Button *buttons,
                            unsigned int button_count,
                            void (*escape_handler)(void *),
                            void *escape_handler_arg,
                            InputState *input_state);
void draw_interface(struct ncplane *plane, Text *texts, uint32_t text_count, Button *buttons, uint32_t button_count);
void free_interface(Text *texts,
                    uint32_t text_count,
                    Button *buttons,
                    uint32_t button_count,
                    void **escape_handler_arg);
