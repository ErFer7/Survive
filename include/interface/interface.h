#pragma once

#include "interface/button.h"
#include "interface/text.h"

#define DEFINE_INTERFACE(NAME, TEXT_COUNT, BUTTON_COUNT) \
    typedef struct {                                     \
        unsigned int background_color;                   \
        int selected_button;                             \
        Text texts[TEXT_COUNT];                          \
        Button buttons[BUTTON_COUNT];                    \
    } NAME;
