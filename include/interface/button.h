#pragma once

#include "../events.h"
#include "text.h"

typedef struct {
    Text text;
    enum Event event;
} Button;
