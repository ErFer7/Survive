#pragma once

#include "types.h"

struct Vector {
    int x;
    int y;
};

static inline Vector create_vector(int x, int y) {
    Vector result;

    result.x = x;
    result.y = y;

    return result;
}

static inline Vector add_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

static inline Vector sub_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x - w.x;
    result.y = v.y - w.y;

    return result;
}

static inline Vector multiply_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x * w.x;
    result.y = v.y * w.y;

    return result;
}

static inline Vector multiply_vector_by_scalar(Vector v, int a) {
    Vector result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

static inline bool is_non_zero(Vector v) { return v.x != 0 || v.y != 0; }
