#include "utils/perlin_noise.h"

float smooth(float x, float y, int octave, int seed) {
    int intX = (int)x;
    float fracX = x - intX;
    int intY = (int)y;
    float fracY = y - intY;

    float v1 = noise(intX, intY, octave, seed);
    float v2 = noise(intX + 1, intY, octave, seed);
    float v3 = noise(intX, intY + 1, octave, seed);
    float v4 = noise(intX + 1, intY + 1, octave, seed);

    float i1 = interpolate(v1, v2, fracX);
    float i2 = interpolate(v3, v4, fracX);

    return interpolate(i1, i2, fracY);
}

float perlin_noise(float x, float y, float persistence, int octaves, int seed) {
    float total = 0.0;
    float frequency = 1.0;
    float amplitude = 1.0;
    int i = 0;

    for (i = 0; i < octaves; i++) {
        total += smooth(x * frequency, y * frequency, i, seed) * amplitude;
        frequency /= 2;
        amplitude *= persistence;
    }

    return total;
}
