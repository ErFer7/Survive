
#include "utils/vector.h"

#include <math.h>

Vector create_vector(int x, int y) {
    Vector result;

    result.x = x;
    result.y = y;

    return result;
}

VectorF create_vector_f(float x, float y) {
    VectorF result;

    result.x = x;
    result.y = y;

    return result;
}

Vector vector_float2int(VectorF v) {
    Vector result;

    result.x = (int)v.x;
    result.y = (int)v.y;

    return result;
}

VectorF vector_int2float(Vector v) {
    VectorF result;

    result.x = (float)v.x;
    result.y = (float)v.y;

    return result;
}

Vector add_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

VectorF add_vector_f(VectorF v, VectorF w) {
    VectorF result;

    result.x = v.x + w.x;
    result.y = v.y + w.y;

    return result;
}

Vector sub_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x - w.x;
    result.y = v.y - w.y;

    return result;
}

VectorF sub_vector_f(VectorF v, VectorF w) {
    VectorF result;

    result.x = v.x - w.x;
    result.y = v.y - w.y;

    return result;
}

Vector multiply_vector(Vector v, Vector w) {
    Vector result;

    result.x = v.x * w.x;
    result.y = v.y * w.y;

    return result;
}

VectorF multiply_vector_f(VectorF v, VectorF w) {
    VectorF result;

    result.x = v.x * w.x;
    result.y = v.y * w.y;

    return result;
}

Vector multiply_vector_by_scalar(Vector v, int a) {
    Vector result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

VectorF multiply_vector_f_by_scalar(VectorF v, float a) {
    VectorF result;

    result.x = v.x * a;
    result.y = v.y * a;

    return result;
}

float distance_f(VectorF v, VectorF w) { return sqrtf(powf((v.x - w.x), 2.0f) + powf((v.y - w.y), 2.0f)); }

float distance(Vector v, Vector w) {
    VectorF vf = vector_int2float(v);
    VectorF wf = vector_int2float(w);

    return distance_f(vf, wf);
}
