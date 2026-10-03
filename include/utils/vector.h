#pragma once

#include "types.h"

struct VectorF {
    float x;
    float y;
};

struct Vector {
    int x;
    int y;
};

Vector create_vector(int x, int y);
VectorF create_vector_f(float x, float y);

Vector vector_float2int(VectorF v);
VectorF vector_int2float(Vector v);
Vector add_vector(Vector v, Vector w);
VectorF add_vector_f(VectorF v, VectorF w);
Vector sub_vector(Vector v, Vector w);
VectorF sub_vector_f(VectorF v, VectorF w);
Vector multiply_vector(Vector v, Vector w);
VectorF multiply_vector_f(VectorF v, VectorF w);
Vector multiply_vector_by_scalar(Vector v, int a);
VectorF multiply_vector_f_by_scalar(VectorF v, float a);
float distance_f(VectorF v, VectorF w);
float distance(Vector v, Vector w);
