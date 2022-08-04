#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"
#include "../include/world.h"

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
    int intx = (int)x;
    double fracx = x - intx;
    int inty = (int)y;
    double fracy = y - inty;

    double v1 = Noise(intx, inty, octave, seed);
    double v2 = Noise(intx + 1, inty, octave, seed);
    double v3 = Noise(intx, inty + 1, octave, seed);
    double v4 = Noise(intx + 1, inty + 1, octave, seed);

    double i1 = Interpolate(v1, v2, fracx);
    double i2 = Interpolate(v3, v4, fracx);

    return Interpolate(i1, i2, fracy);
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

void GenerateWorld(unsigned int width, unsigned int height)
{
    /* Gera o mundo do jogo.
     */

    score = 0;
    idCount = 0;

    // Inicializa as matrizes
    InitEntityMatrix(width, height);

    int seed = (int)Randomf(0, 2147483647);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Entity wall = {idCount++,
                           {219},
                           0.0f,
                           0.0f,
                           0,
                           0x07,
                           {0.0f, 0.0f},
                           {(float)j, (float)i},
                           0.0f,
                           WALL};

            double noise = PerlinNoise((double)j * 0.1, (double)i * 0.1, 0.65, 5, seed);

            if (noise > 0.7 && noise <= 0.775)
            {
                wall.c[0] = 176;
                InsertEntityOnMatrix(wall, (int)wall.position[0], (int)wall.position[1]);
            } else if (noise > 0.775 && noise <= 0.85)
            {
                wall.c[0] = 177;
                InsertEntityOnMatrix(wall, (int)wall.position[0], (int)wall.position[1]);
            } else if (noise > 0.85 && noise <= 0.925)
            {
                wall.c[0] = 178;
                InsertEntityOnMatrix(wall, (int)wall.position[0], (int)wall.position[1]);
            } else if (noise > 0.925)
            {
                InsertEntityOnMatrix(wall, (int)wall.position[0], (int)wall.position[1]);
            }
        }
    }

    // Constroi as paredes de cima e de baixo
    for (int i = 0; i < width; i++)
    {
        Entity topWall = {idCount++,
                          {219},
                          0.0f,
                          0.0f,
                          0,
                          0x87,
                          {0.0f, 0.0f},
                          {(float)i, 0.0f},
                          0.0f,
                          WALL};

        Entity bottonWall = {idCount++,
                             {219},
                             0.0f,
                             0.0f,
                             0,
                             0x87,
                             {0.0f, 0.0f},
                             {(float)i, (float)(height - 1)},
                             0.0f,
                             WALL};

        InsertEntityOnMatrix(topWall,
                             (int)topWall.position[0],
                             (int)topWall.position[1]);

        InsertEntityOnMatrix(bottonWall,
                             (int)bottonWall.position[0],
                             (int)bottonWall.position[1]);
    }

    // Constroi as paredes da esquerda e direita
    for (int i = 0; i < height - 1; i++)
    {
        Entity leftWall = {idCount++,
                           {219},
                           0.0f,
                           0.0f,
                           0,
                           0x87,
                           {0.0f, 0.0f},
                           {0.0f, (float)i},
                           0.0f,
                           WALL};

        Entity rightWall = {idCount++,
                            {219},
                            0.0f,
                            0.0f,
                            0,
                            0x87,
                            {0.0f, 0.0f},
                            {(float)(width - 1), (float)i},
                            0.0f,
                            WALL};

        InsertEntityOnMatrix(leftWall,
                             (int)leftWall.position[0],
                             (int)leftWall.position[1]);

        InsertEntityOnMatrix(rightWall,
                             (int)rightWall.position[0],
                             (int)rightWall.position[1]);
    }

    float spawnX = 0.0f;
    float spawnY = 0.0f;

    do
    {
        spawnX = Randomf(1, width - 2);
        spawnY = Randomf(1, height - 2);
    } while (GetEntityPtrFromMatrix((int)spawnX, (int)spawnY)->type != EMPTY);

    Entity player = {idCount++,
                     {254},
                     0.0f,
                     ANIMATION_SPEED,
                     0,
                     0x0F,
                     {0.0f, 0.0f},
                     {spawnX, spawnY},
                     PLAYER_SPEED,
                     PLAYER};

    InsertEntityOnMatrix(player,
                         (int)player.position[0],
                         (int)player.position[1]);

    for (int i = 0; i < MAX_COINS; i++)
    {
        // Não é a maneira ideal de evitar a sobreposição da moeda no jogador
        do
        {
            spawnX = Randomf(1, width - 2);
            spawnY = Randomf(1, height - 2);
        } while (GetEntityPtrFromMatrix((int)spawnX, (int)spawnY)->type != EMPTY);

        Entity coin = {idCount++,
                       {45, 92, 124, 47},
                       0.0f,
                       ANIMATION_SPEED,
                       1,
                       0x0E,
                       {0.0f, 0.0f},
                       {spawnX, spawnY},
                       0.0f,
                       COIN};

        // Insere o jogador e moeda na matriz

        InsertEntityOnMatrix(coin,
                             (int)coin.position[0],
                             (int)coin.position[1]);
    }

}
