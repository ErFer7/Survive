#pragma once

double RawNoise(int n);
double Noise(int x, int y, int octave, int seed);
double Interpolate(double a, double b, double x);
double Smooth(double x, double y, int octave, int seed);
double PerlinNoise(double x, double y, double persistence, int octaves, int seed);
void GenerateWorld(unsigned int width, unsigned int height);
