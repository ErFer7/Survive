#pragma once

#include <stdlib.h>

static inline int32_t inclusive_random(const int32_t min, const int32_t max) {
    return (rand() % (max - min + 1)) + min;
}
