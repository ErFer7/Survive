#include "../include/world.h"

#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"

#define PI 3.141593

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

void GenerateWorld(GameplayContext *gameplayCtxPtr)
{
    /* Gera o mundo do jogo.
     */

    if (!gameplayCtxPtr->empty)
    {
        int seed = (int)Randomf(0, 2147483647);

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < gameplayCtxPtr->worldSize.y; i++)
        {
            for (int j = 0; j < gameplayCtxPtr->worldSize.x; j++)
            {
                Vector2D position = CreateVector2D(j, i);
                double noise = PerlinNoise((double)j * 0.1, (double)i * 0.1, 0.65, 5, seed);

                if (noise > 0.7 && noise <= 0.775)
                {
                    Entity wall = CreateWall(position, 176);
                    InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, wall, position);
                }
                else if (noise > 0.775 && noise <= 0.85)
                {
                    Entity wall = CreateWall(position, 177);
                    InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, wall, position);
                }
                else if (noise > 0.85 && noise <= 0.925)
                {
                    Entity wall = CreateWall(position, 178);
                    InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, wall, position);
                }
                else if (noise > 0.925)
                {
                    Entity wall = CreateWall(position, 219);
                    InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, wall, position);
                }
            }
        }
    }

    // Constroi as paredes de cima e de baixo
    for (int i = 0; i < gameplayCtxPtr->worldSize.x; i++)
    {
        Vector2D topPosition = CreateVector2D(i, 0);
        Vector2D bottomPosition = CreateVector2D(i, gameplayCtxPtr->worldSize.y - 1);

        Entity topWall = CreateWall(topPosition, 219);
        Entity bottomWall = CreateWall(bottomPosition, 219);

        InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, topWall, topPosition);
        InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, bottomWall, bottomPosition);
    }

    // Constroi as paredes da esquerda e direita
    for (int i = 0; i < gameplayCtxPtr->worldSize.y - 1; i++)
    {
        Vector2D leftPosition = CreateVector2D(0, i);
        Vector2D rightPosition = CreateVector2D(gameplayCtxPtr->worldSize.x - 1, i);

        Entity leftWall = CreateWall(leftPosition, 219);
        Entity rightWall = CreateWall(rightPosition, 219);

        InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, leftWall, leftPosition);
        InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, rightWall, rightPosition);
    }

    Vector2D spawn;

    spawn = GenerateSpawnPosition(&gameplayCtxPtr->entityMatrix, 0.0f, CreateVector2D(0, 0));
    Entity player = CreatePlayer(spawn);
    InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, player, spawn);

    // Para um mapa 120 x 30 era gerada apenas uma moeda, sendo assim 3600 equivalem a uma moeda. Usando a regra de 3
    // obtém-se o limite abaixo
    for (int i = 0; i < (int)round((gameplayCtxPtr->worldSize.x * gameplayCtxPtr->worldSize.y) / 3600.0f); i++)
    {
        spawn = GenerateSpawnPosition(&gameplayCtxPtr->entityMatrix, 0.0f, CreateVector2D(0, 0));
        Entity coin = CreateCoin(spawn);
        InsertEntityOnMatrix(&gameplayCtxPtr->entityMatrix, coin, spawn);
    }
}
