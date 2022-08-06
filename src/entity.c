#include "../include/entity.h"

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <windows.h>

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

EntityMatrix entityMatrix;
unsigned int idCount;
pthread_t renderingThread;

void InitEntityMatrix(int width, int height)
{
    /* Inicializa uma matriz de entidades. A matriz é alocada na memória e possui uma largura e
       altura. Existem dois ponteiros que são úteis para manter a referência do jogador e
       moeda.
    */

    entityMatrix.matrix = (Entity *)malloc(width * height * sizeof(Entity));
    entityMatrix.width = width;
    entityMatrix.height = height;
    entityMatrix.coinPtrsSize = 0;
    entityMatrix.enemyPtrsSize = 0;
    entityMatrix.allocated = 1;

    // Preenche a matriz com entidades vazios
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
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

void InsertEntityOnMatrix(Entity entity, int x, int y)
{
    /* Insere uma entidade em uma matriz (sobescreve a entidade caso já exista uma na posição).
     */

    entityMatrix.matrix[entityMatrix.width * y + x] = entity;

    if (entity.type == PLAYER)
    {
        entityMatrix.playerPtr = &entityMatrix.matrix[entityMatrix.width * y + x];
    }
    else if (entity.type == COIN)
    {
        entityMatrix.coinPtrs[entityMatrix.coinPtrsSize] = &entityMatrix.matrix[entityMatrix.width * y + x];
        entityMatrix.coinPtrsSize++;
    }
    else if (entity.type == ENEMY)
    {
        if (entityMatrix.enemyPtrsSize == 0)
        {
            entityMatrix.enemyPtrs = (Entity **)malloc(sizeof(Entity *));
        }
        else
        {
            entityMatrix.enemyPtrs = (Entity **)realloc(entityMatrix.enemyPtrs,
                                                        sizeof(Entity *) * (entityMatrix.enemyPtrsSize + 1));
        }

        entityMatrix.enemyPtrsSize++;
        entityMatrix.enemyPtrs[entityMatrix.enemyPtrsSize - 1] = &entityMatrix.matrix[entityMatrix.width * y + x];
    }
}

void MoveEntityOnMatrix(int x0, int y0, int x1, int y1)
{
    /* Move um objeto na matriz caso as posições fornecidas sejam válidas. Sobescreve o objeto em
       (x1, y1).
    */

    if (x0 != x1 || y0 != y1)
    {
        Entity empty = {7, {255}, 0.0f, 0.0f, 0, 0, {0.0f, 0.0f}, {(float)x0, (float)y0}, 0.0f, EMPTY};
        Entity entity = entityMatrix.matrix[entityMatrix.width * y0 + x0];

        entityMatrix.matrix[entityMatrix.width * y1 + x1] = entity;

        if (entity.type == PLAYER)
        {
            entityMatrix.playerPtr = &entityMatrix.matrix[entityMatrix.width * y1 + x1];
        }
        else if (entity.type == COIN)
        {
            for (int i = 0; i < entityMatrix.coinPtrsSize; i++)
            {
                if (entity.id == entityMatrix.coinPtrs[i]->id)
                {
                    entityMatrix.coinPtrs[i] = &entityMatrix.matrix[entityMatrix.width * y1 + x1];
                }
            }
        }
        else if (entity.type == ENEMY)
        {
            for (int i = 0; i < entityMatrix.enemyPtrsSize; i++)
            {
                if (entity.id == entityMatrix.enemyPtrs[i]->id)
                {
                    entityMatrix.enemyPtrs[i] = &entityMatrix.matrix[entityMatrix.width * y1 + x1];
                }
            }
        }

        entityMatrix.matrix[entityMatrix.width * y0 + x0] = empty;
    }
}

Entity *GetEntityPtrFromMatrix(int x, int y)
{
    /* Retorna um ponteiro para a entidade na posição especificada.
     */

    return &entityMatrix.matrix[entityMatrix.width * y + x];
}

void FreeEntityMatrix()
{
    /* Libera a memória da matriz.
     */

    if (entityMatrix.allocated)
    {
        free(entityMatrix.enemyPtrs);
        free(entityMatrix.matrix);
        entityMatrix.enemyPtrs = NULL;
        entityMatrix.matrix = NULL;
        entityMatrix.width = 0;
        entityMatrix.height = 0;
        entityMatrix.allocated = 0;
    }

}

void UpdateEntityBehaviour()
{
    /* Atualiza cada objeto com base no seu comportamento.
     */

    PlayerBehaviour();

    for (int i = 0; i < entityMatrix.enemyPtrsSize; i++)
    {
        EnemyBehaviour(entityMatrix.enemyPtrs[i]);
    }
}

void PlayerBehaviour()
{
    /* Comportamento do jogador.
     */

    float runCoefficient = 1.0f; // Coeficiente de corrida

    if (GetKeyState(VK_SPACE) & 0x8000) // Espaço
        runCoefficient = 2.0f;          // Dobra a velocidade na corrida

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
        int expectedPositionX = (int)(entityMatrix.playerPtr->position[0] +
                                      entityMatrix.playerPtr->velocity[0] / (float)tick);

        int expectedPositionY = (int)(entityMatrix.playerPtr->position[1] +
                                      entityMatrix.playerPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (int)entityMatrix.playerPtr->position[0] &&
            expectedPositionY != (int)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[1] = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPositionX != (int)entityMatrix.playerPtr->position[0] &&
            expectedPositionY == (int)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[0] = 0.0f;
    }
}

void EnemyBehaviour(Entity *enemyPtr)
{
    /* Comportamento do inimigo.
     */

    int intents[4] = {0, 0, 0, 0};

    int x = enemyPtr->position[0];
    int y = enemyPtr->position[1];

    if ((int)entityMatrix.playerPtr->position[0] > x) // Caso o jogador esteja na direita
    {
        intents[0] = 1;
    }
    else if ((int)entityMatrix.playerPtr->position[0] < x) // Caso o jogador esteja na esquerda
    {
        intents[1] = 1;
    }

    if ((int)entityMatrix.playerPtr->position[1] > y) // Caso o jogador esteja em baixo
    {
        intents[2] = 1;
    }
    else if ((int)entityMatrix.playerPtr->position[1] < y) // Caso o jogador esteja em cima
    {
        intents[3] = 1;
    }

    if (intents[0])
    {
        enemyPtr->velocity[0] = enemyPtr->speed;
    }
    else if (intents[1])
    {
        enemyPtr->velocity[0] = -enemyPtr->speed;
    }

    if (intents[2])
    {
        enemyPtr->velocity[1] = enemyPtr->speed;
    }
    else if (intents[3])
    {
        enemyPtr->velocity[1] = -enemyPtr->speed;
    }

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (enemyPtr->velocity[0] != 0.0f && enemyPtr->velocity[1] != 0.0f)
    {
        // Obtém a posição esperada
        int expectedPositionX = (int)(enemyPtr->position[0] +
                                      enemyPtr->velocity[0] / (float)tick);
        int expectedPositionY = (int)(enemyPtr->position[1] +
                                      enemyPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (int)enemyPtr->position[0] &&
            expectedPositionY != (int)enemyPtr->position[1])
            enemyPtr->velocity[1] = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPositionX != (int)enemyPtr->position[0] &&
            expectedPositionY == (int)enemyPtr->position[1])
            enemyPtr->velocity[0] = 0.0f;
    }
}

void UpdateEntityPhysics()
{
    /* Atualiza a física e verifica os eventos do gameplay.
     */

    UpdatePlayerPhysics();

    for (int i = 0; i < entityMatrix.enemyPtrsSize; i++)
    {
        UpdateEnemyPhysics(entityMatrix.enemyPtrs[i]);
    }
}

void UpdatePlayerPhysics()
{
    int oldPositionX;
    int oldPositionY;
    float targetPositionX;
    float targetPositionY;
    Entity *entityPtrInTargetPosition;
    int oldCoinPositionX;
    int oldCoinPositionY;
    float spawnX;
    float spawnY;
    Entity *entityPtrInSpawnPosition;
    float distanceFromPlayer;

    // Obtém a posição
    oldPositionX = (int)entityMatrix.playerPtr->position[0];
    oldPositionY = (int)entityMatrix.playerPtr->position[1];

    // Obtém a posição alvo
    targetPositionX = entityMatrix.playerPtr->position[0] + entityMatrix.playerPtr->velocity[0] / (float)tick;
    targetPositionY = entityMatrix.playerPtr->position[1] + entityMatrix.playerPtr->velocity[1] / (float)tick;

    // Redefine a velocidade
    entityMatrix.playerPtr->velocity[0] = 0.0f;
    entityMatrix.playerPtr->velocity[1] = 0.0f;

    // Ponteiro do objeto na posição alvo
    entityPtrInTargetPosition = GetEntityPtrFromMatrix((int)targetPositionX, (int)targetPositionY);

    /* Move o jogador caso o objeto na posição alvo esteja vazio ou seja o próprio
        jogador
    */
    if (entityPtrInTargetPosition->type == EMPTY || entityPtrInTargetPosition->id == entityMatrix.playerPtr->id)
    {
        // Define a nova posição
        entityMatrix.playerPtr->position[0] = targetPositionX;
        entityMatrix.playerPtr->position[1] = targetPositionY;

        // Move o jogador na matriz
        MoveEntityOnMatrix(oldPositionX,
                           oldPositionY,
                           (int)entityMatrix.playerPtr->position[0],
                           (int)entityMatrix.playerPtr->position[1]);
    }
    // Caso o jogador esteja pegando a moeda
    else if (entityPtrInTargetPosition->type == COIN)
    {
        score++;

        // Atualiza o texto da pontuação
        sprintf(gameplay.texts[2].content, "Score: %010d", score);
        gameplay.texts[2].update = 1;

        // Obtém a posição da moeda
        oldCoinPositionX = (int)entityPtrInTargetPosition->position[0];
        oldCoinPositionY = (int)entityPtrInTargetPosition->position[1];

        /* Não é a maneira ideal de evitar a sobreposição da moeda em outros
            objetos
        */
        do
        {
            /* Gera uma nova posição e repete o processo caso tenha algo na posição
                gerada
            */

            spawnX = Randomf(1, entityMatrix.width - 2);
            spawnY = Randomf(1, entityMatrix.height - 2);
            entityPtrInSpawnPosition = GetEntityPtrFromMatrix((int)spawnX, (int)spawnY);
        } while (entityPtrInSpawnPosition->type != EMPTY);

        // Define a nova posição da moeda
        entityPtrInTargetPosition->position[0] = spawnX;
        entityPtrInTargetPosition->position[1] = spawnY;

        // Move a moeda na matriz
        MoveEntityOnMatrix(oldCoinPositionX,
                            oldCoinPositionY,
                            (int)spawnX,
                            (int)spawnY);

        /* Gera a posição do novo inimigo. A posição não pode sobrepor nenhum
            objeto e deve manter uma distância do jogador.
        */
        do
        {
            spawnX = Randomf(1, entityMatrix.width - 2);
            spawnY = Randomf(1, entityMatrix.height - 2);
            entityPtrInSpawnPosition = GetEntityPtrFromMatrix((int)spawnX, (int)spawnY);

            distanceFromPlayer = sqrtf(powf((spawnX - targetPositionX), 2.0f) +
                                       powf((spawnY - targetPositionY), 2.0f));

        } while (entityPtrInSpawnPosition->type != EMPTY || distanceFromPlayer < 20.0f);

        Entity enemy = {idCount++,
                        {178, 177, 176, 178},
                        0.0f,
                        ANIMATION_SPEED,
                        1,
                        0x0C,
                        {0.0f, 0.0f},
                        {spawnX, spawnY},
                        ENEMY_SPEED,
                        ENEMY};

        // Insere o inimigo na matriz
        InsertEntityOnMatrix(enemy, spawnX, spawnY);

        // Move o jogador
        entityMatrix.playerPtr->position[0] = targetPositionX;
        entityMatrix.playerPtr->position[1] = targetPositionY;

        MoveEntityOnMatrix(oldPositionX,
                           oldPositionY,
                           (int)entityMatrix.playerPtr->position[0],
                           (int)entityMatrix.playerPtr->position[1]);
    }
}

void UpdateEnemyPhysics(Entity *enemyPtr)
{
    int oldPositionX;
    int oldPositionY;
    float targetPositionX;
    float targetPositionY;
    Entity *entityPtrInTargetPosition;

    // Obtém a posição
    oldPositionX = (int)enemyPtr->position[0];
    oldPositionY = (int)enemyPtr->position[1];

    // Obtém a posição alvo
    targetPositionX = enemyPtr->position[0] + enemyPtr->velocity[0] / (float)tick;
    targetPositionY = enemyPtr->position[1] + enemyPtr->velocity[1] / (float)tick;

    // Redefine a velocidade
    enemyPtr->velocity[0] = 0.0f;
    enemyPtr->velocity[1] = 0.0f;

    // Ponteiro do objeto na posição alvo
    entityPtrInTargetPosition = GetEntityPtrFromMatrix((int)targetPositionX, (int)targetPositionY);

    // Se move caso o a posição alvo não tenha um objeto ou tenha o próprio inimigo
    if (entityPtrInTargetPosition->type == EMPTY || entityPtrInTargetPosition->id == enemyPtr->id)
    {
        enemyPtr->position[0] = targetPositionX;
        enemyPtr->position[1] = targetPositionY;

        MoveEntityOnMatrix(oldPositionX,
                           oldPositionY,
                           (int)targetPositionX,
                           (int)targetPositionY);
    }
    // Caso a posição tenha o jogador
    else if (entityPtrInTargetPosition->type == PLAYER)
    {
        events[0] = GM_GAMEOVER; // Adiciona o evento de fim de jogo
        sprintf(gameover.texts[1].content, "Score: %010d", score);
    }
}

void StartRenderingThread()
{
    pthread_create(&renderingThread, NULL, RenderEntities, NULL);
}

void StopRenderingThread()
{
    pthread_join(renderingThread, NULL);
}

void *RenderEntities()
{
    /* Rederiza as entidades.
     */

    int validEntity;
    int x, y, xOffset, yOffset;
    Entity *entityPtr;

    while (state == GAMEPLAY)
    {
        StartChronometer(&renderingFrequency, &renderingInitialTime);

        xOffset = (int)(entityMatrix.playerPtr->position[0] - 0.5 * consoleWidth);
        yOffset = (int)(entityMatrix.playerPtr->position[1] - 0.5 * consoleHeight);

        for (int i = 0; i < consoleHeight - 1; i++)
        {
            for (int j = 0; j < consoleWidth; j++)
            {
                x = j + xOffset;
                y = i + yOffset;

                if ((x >= 0 && x < entityMatrix.width) && (y >= 0 && y < entityMatrix.height))
                {
                    entityPtr = GetEntityPtrFromMatrix(x, y);
                    validEntity = entityPtr->type != EMPTY ? 1 : 0;
                }
                else
                {
                    validEntity = 0;
                }

                // Limpa o caractere caso ela esteja vazio na matriz
                if (!validEntity)
                {
                    SetCharOnPosition(j, i, 32, 0x00);
                }
                else // Coloca o caractere
                {
                    int isAnimated = entityPtr->isAnimated;
                    char newChar;

                    if (isAnimated)
                    {
                        entityPtr->animationFrame += ANIMATION_SPEED * (float)renderingElapsedTime;

                        if ((int)entityPtr->animationFrame > MAX_ANIM_FRAMES - 1)
                            entityPtr->animationFrame = 0.0f;

                        newChar = entityPtr->c[(int)entityPtr->animationFrame];
                    }
                    else
                    {
                        newChar = entityPtr->c[0];
                    }

                    SetCharOnPosition(j, i, newChar, entityPtr->color);
                }
            }
        }

        WriteOutput();

        renderingElapsedTime = StopChronometer(renderingFrequency, renderingInitialTime, &renderingFinalTime);

        sprintf(gameplay.texts[0].content, "%012.3f fps", 1000.0f / (float)renderingElapsedTime);
        gameplay.texts[0].update = 1;
    }

    return 0;
}
