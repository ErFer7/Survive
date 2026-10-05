#pragma once

#include <math.h>

static const float PI = 3.141593f;

// Thanks Charles Zinn for the perlin noise generator
static inline float raw_noise(int n) {
    n = (n << 13) ^ n;

    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

static inline float noise(int x, int y, int octave, int seed) {
    return raw_noise(x * 1619 + y * 31337 + octave * 3463 + seed * 13397);
}

static inline float interpolate(float a, float b, float x) {
    float f = (1 - cosf(x * PI)) * 0.5f;

    return a * (1 - f) + b * f;
}

float smooth(float x, float y, int octave, int seed);
float perlin_noise(float x, float y, float persistence, int octaves, int seed);
