#include "../include/entity.h"

#include <stdio.h>
#include <math.h>
#include <windows.h>

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

EntityMatrix entityMatrix;
unsigned int idCount;

void EntityMatrixInit(unsigned int width, unsigned int height)
{
    /* Inicializa uma matriz de entidades. A matriz é alocada na memória e possui uma largura e
       altura. Existem dois ponteiros que são úteis para manter a referência do jogador e
       moeda.
    */

    entityMatrix.matrix = (Entity *)malloc(width * height * sizeof(Entity));
    entityMatrix.width = width;
    entityMatrix.height = height;

    // Preenche a matriz com entidades vazios
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            Entity empty = {0,
                            {255},
                            0.0f,
                            0.0f,
                            0,
                            0x00,
                            {0.0f, 0.0f},
                            {(float)i, (float)j},
                            0.0f,
                            EMPTY};

            InsertEntityOnMatrix(empty, j, i);
        }
    }
}

void InsertEntityOnMatrix(Entity entity, unsigned int x, unsigned int y)
{
    /* Insere uma entidade em uma matriz (sobescreve a entidade caso já exista uma na posição).
     */

    entityMatrix.matrix[entityMatrix.width * y + x] = entity;

    if (entity.type == PLAYER)
        entityMatrix.playerPtr = &entityMatrix.matrix[entityMatrix.width * y + x];
    else if (entity.type == COIN)
        entityMatrix.coinPtr = &entityMatrix.matrix[entityMatrix.width * y + x];
}

void MoveEntityOnMatrix(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    /* Move um objeto na matriz caso as posições fornecidas sejam válidas. Sobescreve o objeto em
       (x1, y1).
    */

    if (x0 != x1 || y0 != y1)
    {
        Entity empty = {0, {255}, 0.0f, 0.0f, 0, 0, {0.0f, 0.0f}, {(float)x0, (float)y0}, 0.0f, EMPTY};
        Entity object = entityMatrix.matrix[entityMatrix.width * y0 + x0];

        entityMatrix.matrix[entityMatrix.width * y1 + x1] = object;
        entityMatrix.matrix[entityMatrix.width * y0 + x0] = empty;

        if (object.type == PLAYER)
            entityMatrix.playerPtr = &entityMatrix.matrix[entityMatrix.width * y1 + x1];
        else if (object.type == COIN)
            entityMatrix.coinPtr = &entityMatrix.matrix[entityMatrix.width * y1 + x1];
    }
}

Entity *GetEntityPtrFromMatrix(unsigned int x, unsigned int y)
{
    /* Retorna um ponteiro para a entidade na posição especificada.
     */

    return &entityMatrix.matrix[entityMatrix.width * y + x];
}

void FreeEntityMatrix()
{
    /* Libera a memória da matriz.
     */

    free(entityMatrix.matrix);
    entityMatrix.matrix = NULL;
    entityMatrix.width = 0;
    entityMatrix.height = 0;
}

void UpdateObjectBehaviour()
{
    /* Atualiza cada objeto com base no seu comportamento.
     */

    PlayerBehaviour();

    for (unsigned int i = 0; i < entityMatrix.height; i++)
        for (unsigned int j = 0; j < entityMatrix.width; j++)
            if (GetEntityPtrFromMatrix(j, i)->type == ENEMY)
                EnemyBehaviour(j, i);
}

void PlayerBehaviour()
{
    /* Comportamento do jogador.
     */

    float runCoefficient = 1.0f; // Coeficiente de corrida

    if (GetKeyState(VK_SPACE) & 0x8000) // Espaço
        runCoefficient = 2.0f; // Dobra a velocidade na corrida

    if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
        // Adiciona velocidade para cima
        entityMatrix.playerPtr->velocity[1] = -entityMatrix.playerPtr->speed * runCoefficient;
    else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
        // Adiciona velocidade para baixo
        entityMatrix.playerPtr->velocity[1] = entityMatrix.playerPtr->speed * runCoefficient;

    if (GetKeyState(VK_RIGHT) & 0x8000) // Seta para a direita
        // Adiciona velocidade para direita
        entityMatrix.playerPtr->velocity[0] = entityMatrix.playerPtr->speed * runCoefficient;
    else if (GetKeyState(VK_LEFT) & 0x8000) // Seta para a esquerda
        // Adiciona velocidade para esquerda
        entityMatrix.playerPtr->velocity[0] = -entityMatrix.playerPtr->speed * runCoefficient;

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (entityMatrix.playerPtr->velocity[0] != 0.0f && entityMatrix.playerPtr->velocity[1] != 0.0f)
    {
        // Obtém a posição esperada
        unsigned int expectedPositionX = (unsigned int)(entityMatrix.playerPtr->position[0] +
                                                        entityMatrix.playerPtr->velocity[0] / (float)tick);

        unsigned int expectedPositionY = (unsigned int)(entityMatrix.playerPtr->position[1] +
                                                        entityMatrix.playerPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (unsigned int)entityMatrix.playerPtr->position[0] &&
            expectedPositionY != (unsigned int)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[1] = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPositionX != (unsigned int)entityMatrix.playerPtr->position[0] &&
            expectedPositionY == (unsigned int)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[0] = 0.0f;
    }
}

void EnemyBehaviour(unsigned int x, unsigned int y)
{
    /* Comportamento do inimigo.
     */

    Entity *enemyPtr = GetEntityPtrFromMatrix(x, y); // Ponteiro do inimigo

    if ((unsigned int)entityMatrix.playerPtr->position[0] > x) // Caso o jogador esteja na direita
        // Adiciona velocidade para cima
        enemyPtr->velocity[0] = enemyPtr->speed;
    else if ((unsigned int)entityMatrix.playerPtr->position[0] < x) // Caso o jogador esteja na esquerda
        // Adiciona velocidade para baixo
        enemyPtr->velocity[0] = -enemyPtr->speed;

    if ((unsigned int)entityMatrix.playerPtr->position[1] > y) // Caso o jogador esteja em cima
        // Adiciona velocidade para direita
        enemyPtr->velocity[1] = enemyPtr->speed;
    else if ((unsigned int)entityMatrix.playerPtr->position[1] < y) // Caso o jogador esteja em baixo
        // Adiciona velocidade para esquerda
        enemyPtr->velocity[1] = -enemyPtr->speed;

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (enemyPtr->velocity[0] != 0.0f && enemyPtr->velocity[1] != 0.0f)
    {
        // Obtém a posição esperada
        unsigned int expectedPositionX = (unsigned int)(enemyPtr->position[0] +
                                                        enemyPtr->velocity[0] / (float)tick);
        unsigned int expectedPositionY = (unsigned int)(enemyPtr->position[1] +
                                                        enemyPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (unsigned int)enemyPtr->position[0] &&
            expectedPositionY != (unsigned int)enemyPtr->position[1])
            enemyPtr->velocity[1] = 0.0f;


        // Corrige a velocidade horizontal
        if (expectedPositionX != (unsigned int)enemyPtr->position[0] &&
            expectedPositionY == (unsigned int)enemyPtr->position[1])
            enemyPtr->velocity[0] = 0.0f;
    }
}

void UpdatePhysics()
{
    /* Atualiza a física e verifica os eventos do gameplay.
     */

    Entity *objectPtr;
    Entity *objectPtrInTargetPosition;
    Entity *objectPtrInCoinPosition;
    Entity *objectPtrInEnemyPosition;
    unsigned int oldPositionX;
    unsigned int oldPositionY;
    float targetPositionX;
    float targetPositionY;
    unsigned int oldCoinPositionX;
    unsigned int oldCoinPositionY;
    float coinSpawnX;
    float coinSpawnY;
    float enemySpawnX;
    float enemySpawnY;
    float distanceFromPlayer;

    for (unsigned int i = 0; i < entityMatrix.height; i++)
    {
        for (unsigned int j = 0; j < entityMatrix.width; j++)
        {
            // Obtém o ponteiro para o objeto
            objectPtr = GetEntityPtrFromMatrix(j, i);

            // Obtém a posição
            oldPositionX = (unsigned int)objectPtr->position[0];
            oldPositionY = (unsigned int)objectPtr->position[1];

            // Obtém a posição alvo
            targetPositionX = objectPtr->position[0] + objectPtr->velocity[0] / (float)tick;
            targetPositionY = objectPtr->position[1] + objectPtr->velocity[1] / (float)tick;

            // Redefine a velocidade
            objectPtr->velocity[0] = 0.0f;
            objectPtr->velocity[1] = 0.0f;

            // Ponteiro do objeto na posição alvo
            objectPtrInTargetPosition = GetEntityPtrFromMatrix((unsigned int)targetPositionX,
                                                               (unsigned int)targetPositionY);
            if (objectPtr->type == PLAYER) // Se for o jogador
            {
                /* Move o jogador caso o objeto na posição alvo esteja vazio ou seja o próprio
                    jogador
                */
                if (objectPtrInTargetPosition->type == EMPTY ||
                    objectPtrInTargetPosition->id == objectPtr->id)
                {
                    // Define a nova posição
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    // Move o jogador na matriz
                    MoveEntityOnMatrix(oldPositionX,
                                       oldPositionY,
                                       (unsigned int)objectPtr->position[0],
                                       (unsigned int)objectPtr->position[1]);
                }
                // Caso o jogador esteja pegando a moeda
                else if (objectPtrInTargetPosition->type == COIN)
                {
                    score++;

                    // Atualiza o texto da pontuação
                    sprintf(gameplay.texts[1].content, "score: %05d", score);
                    gameplay.texts[1].update = 1;

                    // Obtém a posição da moeda
                    oldCoinPositionX = (unsigned int)entityMatrix.coinPtr->position[0];
                    oldCoinPositionY = (unsigned int)entityMatrix.coinPtr->position[1];

                    /* Não é a maneira ideal de evitar a sobreposição da moeda em outros
                        objetos
                    */
                    do
                    {
                        /* Gera uma nova posição e repete o processo caso tenha algo na posição
                            gerada
                        */

                        coinSpawnX = Randomf(1, entityMatrix.width - 2);
                        coinSpawnY = Randomf(1, entityMatrix.height - 3);

                        objectPtrInCoinPosition = GetEntityPtrFromMatrix((unsigned int)coinSpawnX,
                                                                         (unsigned int)coinSpawnY);
                    } while (objectPtrInCoinPosition->type != EMPTY);

                    entityMatrix.coinPtr->position[0] = coinSpawnX;
                    entityMatrix.coinPtr->position[1] = coinSpawnY;

                    // Move a moeda na matriz
                    MoveEntityOnMatrix(oldCoinPositionX,
                                       oldCoinPositionY,
                                       (unsigned int)entityMatrix.coinPtr->position[0],
                                       (unsigned int)entityMatrix.coinPtr->position[1]);

                    /* Gera a posição do novo inimigo. A posição não pode sobrepor nenhum
                        objeto e deve manter uma distância do jogador.
                    */
                    do
                    {
                        enemySpawnX = Randomf(1, entityMatrix.width - 2);
                        enemySpawnY = Randomf(1, entityMatrix.height - 3);

                        objectPtrInEnemyPosition = GetEntityPtrFromMatrix((unsigned int)enemySpawnX,
                                                                          (unsigned int)enemySpawnY);

                        distanceFromPlayer = sqrtf(powf((enemySpawnX - targetPositionX), 2.0f) +
                                                   powf((enemySpawnY - targetPositionY), 2.0f));

                    } while (objectPtrInEnemyPosition->type != EMPTY || distanceFromPlayer < 20.0f);

                    Entity enemy = {idCount++,
                                    {178, 177, 176, 178},
                                    0.0f,
                                    ANIMATION_SPEED,
                                    1,
                                    0x0C,
                                    {0.0f, 0.0f},
                                    {enemySpawnX, enemySpawnY},
                                    ENEMY_SPEED,
                                    ENEMY};

                    // Insere o inimigo na matriz
                    InsertEntityOnMatrix(enemy, enemySpawnX, enemySpawnY);

                    // Move o jogador
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    MoveEntityOnMatrix(oldPositionX,
                                       oldPositionY,
                                       (unsigned int)objectPtr->position[0],
                                       (unsigned int)objectPtr->position[1]);
                }
            }
            else if (objectPtr->type == ENEMY) // Se for o inimigo
            {
                // Se move caso o a posição alvo não tenha um objeto ou tenha o próprio inimigo
                if (objectPtrInTargetPosition->type == EMPTY ||
                    objectPtrInTargetPosition->id == objectPtr->id)
                {
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    MoveEntityOnMatrix(oldPositionX,
                                       oldPositionY,
                                       (unsigned int)targetPositionX,
                                       (unsigned int)targetPositionY);
                }
                // Caso a posição tenha o jogador
                else if (objectPtrInTargetPosition->type == PLAYER)
                {
                    events[0] = GM_GAMEOVER; // Adiciona o evento de fim de jogo
                    sprintf(gameover.texts[1].content, "score: %05d", score);
                }
            }
        }
    }
}

void RenderEntities(int tick)
{
    /* Rederiza as entidades.
     */

    for (unsigned int i = 0; i < entityMatrix.height; i++)
    {
        for (unsigned int j = 0; j < entityMatrix.width; j++)
        {
            // Limpa o caractere caso ela esteja vazio na matriz
            if (GetEntityPtrFromMatrix(j, i)->type == EMPTY)
            {
                SetCharOnPosition(j, i, 32, 0x00);
            }
            else // Coloca o caractere
            {
                unsigned int newAnimationFrame;
                int isAnimated = GetEntityPtrFromMatrix(j, i)->isAnimated;

                if (isAnimated)
                {
                    GetEntityPtrFromMatrix(j, i)->animationFrame += ANIMATION_SPEED / (float)tick;

                    if ((unsigned int)GetEntityPtrFromMatrix(j, i)->animationFrame > MAX_ANIM_FRAMES - 1)
                        GetEntityPtrFromMatrix(j, i)->animationFrame = 0.0f;
                }

                newAnimationFrame = (unsigned int)GetEntityPtrFromMatrix(j, i)->animationFrame;

                char newChar = GetEntityPtrFromMatrix(j, i)->c[newAnimationFrame];

                SetCharOnPosition(j, i, newChar, GetEntityPtrFromMatrix(j, i)->color);
            }
        }
    }

    WriteOutput();
}
