#include "../include/world.h"

#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"

#define PI 3.141593

int worldWidth, worldHeight, empty;

// Thanks Charles Zinn for the perlin noise generator
double RawNoise(int n)
{
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Noise(int x, int y, int octave, int seed)
{
    return RawNoise(x * 1619 + y * 31337 + octave * 3463 + seed * 13397);
}

double Interpolate(double a, double b, double x)
{
    double f = (1 - cos(x * PI)) * 0.5;
    return a * (1 - f) + b * f;
}

double Smooth(double x, double y, int octave, int seed)
{
    int intX = (int)x;
    double fracX = x - intX;
    int intY = (int)y;
    double fracY = y - intY;

    double v1 = Noise(intX, intY, octave, seed);
    double v2 = Noise(intX + 1, intY, octave, seed);
    double v3 = Noise(intX, intY + 1, octave, seed);
    double v4 = Noise(intX + 1, intY + 1, octave, seed);

    double i1 = Interpolate(v1, v2, fracX);
    double i2 = Interpolate(v3, v4, fracX);

    return Interpolate(i1, i2, fracY);
}

double PerlinNoise(double x, double y, double persistence, int octaves, int seed)
{
    double total = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    int i = 0;

    for (i = 0; i < octaves; i++)
    {
        total += Smooth(x * frequency, y * frequency, i, seed) * amplitude;
        frequency /= 2;
        amplitude *= persistence;
    }

    return total;
}

void GenerateWorld(int worldWidth_, int worldHeight_, int empty_)
{
    /* Gera o mundo do jogo.
     */

    score = 0;
    idCount = 0;
    worldWidth = worldWidth_;
    worldHeight = worldHeight_;
    empty = empty_;

    // Inicializa as matrizes
    InitEntityMatrix(worldWidth, worldHeight);

    if (!empty)
    {
        int seed = (int)Randomf(0, 2147483647);

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < worldHeight; i++)
        {
            for (int j = 0; j < worldWidth; j++)
            {
                double noise = PerlinNoise((double)j * 0.1, (double)i * 0.1, 0.65, 5, seed);

                if (noise > 0.7 && noise <= 0.775)
                {
                    Entity wall = CreateWall((float)j, (float)i, 176);
                    InsertEntityOnMatrix(wall, j, i);
                }
                else if (noise > 0.775 && noise <= 0.85)
                {
                    Entity wall = CreateWall((float)j, (float)i, 177);
                    InsertEntityOnMatrix(wall, j, i);
                }
                else if (noise > 0.85 && noise <= 0.925)
                {
                    Entity wall = CreateWall((float)j, (float)i, 178);
                    InsertEntityOnMatrix(wall, j, i);
                }
                else if (noise > 0.925)
                {
                    Entity wall = CreateWall((float)j, (float)i, 219);
                    InsertEntityOnMatrix(wall, j, i);
                }
            }
        }
    }

    // Constroi as paredes de cima e de baixo
    for (int i = 0; i < worldWidth; i++)
    {
        Entity topWall = CreateWall((float)i, 0.0f, 219);
        Entity bottonWall = CreateWall((float)i, (float)(worldHeight - 1), 219);

        InsertEntityOnMatrix(topWall, i, 0.0f);
        InsertEntityOnMatrix(bottonWall, i, worldHeight - 1);
    }

    // Constroi as paredes da esquerda e direita
    for (int i = 0; i < worldHeight - 1; i++)
    {
        Entity leftWall = CreateWall(0.0f, (float)i, 219);
        Entity rightWall = CreateWall((float)(worldWidth - 1), (float)i, 219);

        InsertEntityOnMatrix(leftWall, 0.0f, i);
        InsertEntityOnMatrix(rightWall, worldWidth - 1, i);
    }

    float spawnX = 0.0f;
    float spawnY = 0.0f;

    GenerateSpawnPosition(&spawnX, &spawnY, 0.0f, 0.0f, 0.0f);
    Entity player = CreatePlayer(spawnX, spawnY);
    InsertEntityOnMatrix(player, (int)spawnX, (int)spawnY);

    // Para um mapa 120 x 30 era gerada apenas uma moeda, sendo assim 3600 equivalem a uma moeda. Usando a regra de 3
    // obtém-se o limite abaixo
    for (int i = 0; i < (int)round((worldWidth * worldHeight) / 3600.0f); i++)
    {
        GenerateSpawnPosition(&spawnX, &spawnY, 0.0f, 0.0f, 0.0f);
        Entity coin = CreateCoin(spawnX, spawnY);
        InsertEntityOnMatrix(coin, (int)spawnX, (int)spawnY);
    }
}
