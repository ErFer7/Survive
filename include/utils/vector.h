#pragma once

#include <math.h>
#include <stdint.h>

#define DEFINE_VECTOR(NAME, TYPE) \
    typedef struct {              \
        TYPE x;                   \
        TYPE y;                   \
    } NAME;

#define DEFINE_CREATE_VECTOR(NAME, TYPE, FUNCTION_NAME)         \
    static inline NAME create_##FUNCTION_NAME(TYPE x, TYPE y) { \
        NAME result = {x, y};                                   \
        return result;                                          \
    }

#define DEFINE_ADD_VECTOR(NAME, TYPE, FUNCTION_NAME)         \
    static inline NAME add_##FUNCTION_NAME(NAME v, NAME w) { \
        NAME result;                                         \
                                                             \
        result.x = v.x + w.x;                                \
        result.y = v.y + w.y;                                \
                                                             \
        return result;                                       \
    }

#define DEFINE_SUB_VECTOR(NAME, TYPE, FUNCTION_NAME)         \
    static inline NAME sub_##FUNCTION_NAME(NAME v, NAME w) { \
        NAME result;                                         \
                                                             \
        result.x = v.x - w.x;                                \
        result.y = v.y - w.y;                                \
                                                             \
        return result;                                       \
    }

#define DEFINE_MULTIPLY_VECTOR(NAME, TYPE, FUNCTION_NAME)         \
    static inline NAME multiply_##FUNCTION_NAME(NAME v, NAME w) { \
        NAME result;                                              \
                                                                  \
        result.x = v.x * w.x;                                     \
        result.y = v.y * w.y;                                     \
                                                                  \
        return result;                                            \
    }

#define DEFINE_MULTIPLY_VECTOR_BY_SCALAR(NAME, TYPE, FUNCTION_NAME)                \
    static inline NAME multiply_##FUNCTION_NAME##_by_scalar(NAME v, TYPE scalar) { \
        v.x *= scalar;                                                             \
        v.y *= scalar;                                                             \
                                                                                   \
        return v;                                                                  \
    }

#define DEFINE_DIVIDE_VECTOR(NAME, TYPE, FUNCTION_NAME)         \
    static inline NAME divide_##FUNCTION_NAME(NAME v, NAME w) { \
        NAME result;                                            \
                                                                \
        result.x = v.x / w.x;                                   \
        result.y = v.y / w.y;                                   \
                                                                \
        return result;                                          \
    }

#define DEFINE_DIVIDE_VECTOR_BY_SCALAR(NAME, TYPE, FUNCTION_NAME)                \
    static inline NAME divide_##FUNCTION_NAME##_by_scalar(NAME v, TYPE scalar) { \
        v.x /= scalar;                                                           \
        v.y /= scalar;                                                           \
                                                                                 \
        return v;                                                                \
    }

#define DEFINE_VECTOR_CONVERSION(NAME, TARGET_TYPE, TARGET_NAME, FUNCTION_NAME, TARGET_FUNCTION_NAME) \
    static inline TARGET_NAME FUNCTION_NAME##_to_##TARGET_FUNCTION_NAME(NAME v) {                     \
        TARGET_NAME result;                                                                           \
                                                                                                      \
        result.x = (TARGET_TYPE)v.x;                                                                  \
        result.y = (TARGET_TYPE)v.y;                                                                  \
                                                                                                      \
        return result;                                                                                \
    }

#define DEFINE_DISTANCE(NAME, FUNCTION_NAME, SAFE_TYPE)            \
    static inline float FUNCTION_NAME##_distance(NAME v, NAME w) { \
        float diff_x = (SAFE_TYPE)v.x - (SAFE_TYPE)w.x;            \
        float diff_y = (SAFE_TYPE)v.y - (SAFE_TYPE)w.y;            \
                                                                   \
        return sqrtf(diff_x * diff_x + diff_y * diff_y);           \
    }

#define DEFINE_IS_ZERO(NAME, FUNCTION_NAME) \
    static inline bool FUNCTION_NAME##_is_zero(NAME v) { return v.x == 0 && v.y == 0; }

DEFINE_VECTOR(Vector, int32_t)
DEFINE_VECTOR(VectorU, uint32_t)
DEFINE_VECTOR(VectorF, float)

DEFINE_CREATE_VECTOR(Vector, int32_t, vector)
DEFINE_CREATE_VECTOR(VectorU, uint32_t, vector_u)
DEFINE_CREATE_VECTOR(VectorF, float, vector_f)

DEFINE_ADD_VECTOR(Vector, int32_t, vector)
DEFINE_ADD_VECTOR(VectorU, uint32_t, vector_u)
DEFINE_ADD_VECTOR(Vector, float, vector_f)

DEFINE_SUB_VECTOR(Vector, int32_t, vector)
DEFINE_SUB_VECTOR(VectorU, uint32_t, vector_u)
DEFINE_SUB_VECTOR(Vector, float, vector_f)

DEFINE_MULTIPLY_VECTOR(Vector, int32_t, vector)
DEFINE_MULTIPLY_VECTOR(VectorU, uint32_t, vector_u)
DEFINE_MULTIPLY_VECTOR(Vector, float, vector_f)

DEFINE_MULTIPLY_VECTOR_BY_SCALAR(Vector, int32_t, vector)
DEFINE_MULTIPLY_VECTOR_BY_SCALAR(VectorU, uint32_t, vector_u)
DEFINE_MULTIPLY_VECTOR_BY_SCALAR(Vector, float, vector_f)

DEFINE_DIVIDE_VECTOR(Vector, int32_t, vector)
DEFINE_DIVIDE_VECTOR(VectorU, uint32_t, vector_u)
DEFINE_DIVIDE_VECTOR(Vector, float, vector_f)

DEFINE_DIVIDE_VECTOR_BY_SCALAR(Vector, int32_t, vector)
DEFINE_DIVIDE_VECTOR_BY_SCALAR(VectorU, uint32_t, vector_u)
DEFINE_DIVIDE_VECTOR_BY_SCALAR(Vector, float, vector_f)

DEFINE_VECTOR_CONVERSION(Vector, float, VectorF, vector, vector_f)
DEFINE_VECTOR_CONVERSION(VectorF, int32_t, Vector, vector_f, vector)
DEFINE_VECTOR_CONVERSION(Vector, uint32_t, VectorU, vector, vector_u)
DEFINE_VECTOR_CONVERSION(VectorU, int32_t, Vector, vector_u, vector)

DEFINE_DISTANCE(Vector, vector, int32_t)
DEFINE_DISTANCE(VectorU, vector_u, int32_t)
DEFINE_DISTANCE(VectorF, vector_f, float)

static const Vector VECTOR_ZERO = {0, 0};
static const VectorU VECTORU_ZERO = {0U, 0U};
static const VectorF VECTOR_F_ZERO = {0.0f, 0.0f};

DEFINE_IS_ZERO(Vector, vector)
DEFINE_IS_ZERO(VectorU, vector_u)
