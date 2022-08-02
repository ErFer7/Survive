#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"
#include "../include/world.h"

void GenerateWorld()
{
    /* Gera o mundo do jogo.
     */

    score = 0;
    idCount = 0;

    // Inicializa as matrizes
    EntityMatrixInit(consoleWidth, consoleHeight - 1);

    Entity player = {idCount++,
                     {254, 254, 254, 254},
                     0.0f,
                     ANIMATION_SPEED,
                     0,
                     0x0F,
                     {0.0f, 0.0f},
                     {(float)(consoleWidth / 2), (float)(consoleHeight / 2)},
                     PLAYER_SPEED,
                     PLAYER};

    float coinPositionX = 0.0f;
    float coinPositionY = 0.0f;

    // Não é a maneira ideal de evitar a sobreposição da moeda no jogador
    do
    {
        coinPositionX = Randomf(1, consoleWidth - 2);
        coinPositionY = Randomf(1, consoleHeight - 3);
    } while (coinPositionX == player.position[0] && coinPositionY == player.position[1]);

    Entity coin = {idCount++,
                   {45, 92, 124, 47},
                   0.0f,
                   ANIMATION_SPEED,
                   1,
                   0x0E,
                   {0.0f, 0.0f},
                   {coinPositionX, coinPositionY},
                   0.0f,
                   COIN};

    // Insere o jogador e moeda na matriz

    InsertEntityOnMatrix(player,
                         (unsigned int)player.position[0],
                         (unsigned int)player.position[1]);

    InsertEntityOnMatrix(coin,
                         (unsigned int)coin.position[0],
                         (unsigned int)coin.position[1]);

    // Constroi as paredes de cima e de baixo
    for (int i = 0; i < consoleWidth; i++)
    {
        Entity topWall = {idCount++,
                          {219, 223, 219, 220},
                          0.0f,
                          0.0f,
                          0,
                          0x87,
                          {0.0f, 0.0f},
                          {(float)i, 0.0f},
                          0.0f,
                          WALL};

        Entity bottonWall = {idCount++,
                             {219, 223, 219, 220},
                             0.0f,
                             0.0f,
                             0,
                             0x87,
                             {0.0f, 0.0f},
                             {(float)i, (float)(consoleHeight - 2)},
                             0.0f,
                             WALL};

        InsertEntityOnMatrix(topWall,
                             (unsigned int)topWall.position[0],
                             (unsigned int)topWall.position[1]);

        InsertEntityOnMatrix(bottonWall,
                             (unsigned int)bottonWall.position[0],
                             (unsigned int)bottonWall.position[1]);
    }

    // Constroi as paredes da esquerda e direita
    for (int i = 0; i < consoleHeight - 2; i++)
    {
        Entity leftWall = {idCount++,
                           {219, 178, 219, 178},
                           0.0f,
                           0.0f,
                           0,
                           0x87,
                           {0.0f, 0.0f},
                           {0.0f, (float)i},
                           0.0f,
                           WALL};

        Entity rightWall = {idCount++,
                            {219, 178, 219, 178},
                            0.0f,
                            0.0f,
                            0,
                            0x87,
                            {0.0f, 0.0f},
                            {(float)(consoleWidth - 1), (float)i},
                            0.0f,
                            WALL};

        InsertEntityOnMatrix(leftWall,
                             (unsigned int)leftWall.position[0],
                             (unsigned int)leftWall.position[1]);

        InsertEntityOnMatrix(rightWall,
                             (unsigned int)rightWall.position[0],
                             (unsigned int)rightWall.position[1]);
    }
}
