#pragma once

#include <math.h>

#include "types.h"

struct VectorF {
    float x;
    float y;
};

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

static inline VectorF create_vector_f(float x, float y) {
    VectorF result;

    result.x = x;
    result.y = y;

    return result;
}

static inline Vector vector_float2int(VectorF v) {
    Vector result;

    result.x = (int)v.x;
    result.y = (int)v.y;

    return result;
}

static inline VectorF vector_int2float(Vector v) {
    VectorF result;

    result.x = (float)v.x;
    result.y = (float)v.y;

    return result;
}

static inline Vector add_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

static inline VectorF add_vector_f(VectorF v, VectorF w) {
    VectorF result;

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

static inline VectorF sub_vector_f(VectorF v, VectorF w) {
    VectorF result;

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

static inline VectorF multiply_vector_f(VectorF v, VectorF w) {
    VectorF result;

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

static inline VectorF multiply_vector_f_by_scalar(VectorF v, float a) {
    VectorF result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

static inline float distance_f(VectorF v, VectorF w) {
    float x_diff = (v.x - w.x);
    float y_diff = (v.y - w.y);

    return sqrtf(x_diff * x_diff + y_diff * y_diff);
}

static inline float distance(Vector v, Vector w) {
    VectorF vf = vector_int2float(v);
    VectorF wf = vector_int2float(w);

    return distance_f(vf, wf);
}
