#include "utils/perlin_noise.h"

float smooth(float x, float y, int32_t octave, int32_t seed) {
    int32_t int_x = (int32_t)x;
    float frac_x = x - int_x;
    int32_t int_y = (int32_t)y;
    float frac_y = y - int_y;

    float v1 = noise(int_x, int_y, octave, seed);
    float v2 = noise(int_x + 1, int_y, octave, seed);
    float v3 = noise(int_x, int_y + 1, octave, seed);
    float v4 = noise(int_x + 1, int_y + 1, octave, seed);

    float i1 = interpolate(v1, v2, frac_x);
    float i2 = interpolate(v3, v4, frac_x);

    return interpolate(i1, i2, frac_y);
}

float perlin_noise(float x, float y, float persistence, int32_t octaves, int32_t seed) {
    float total = 0.0;
    float frequency = 1.0;
    float amplitude = 1.0;
    int32_t i = 0;

    for (i = 0; i < octaves; i++) {
        total += smooth(x * frequency, y * frequency, i, seed) * amplitude;
        frequency /= 2;
        amplitude *= persistence;
    }

    return total;
}
