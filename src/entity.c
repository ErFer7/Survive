#include "../include/entity.h"

#include <math.h>
#include <pthread.h>
#include <windows.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

void PreInitThreadsContext(ThreadsContext *threadsCtxPtr)
{
    threadsCtxPtr->updateThreadRunning = 0;
    threadsCtxPtr->renderingThreadRunning = 0;
}

/*  Pré inicialização do contexto de gameplay
*/
void PreInitGameplayContext(GameplayContext *gameplayCtxPtr)
{
    gameplayCtxPtr->entityMatrix.matrixAllocated = 0;
    gameplayCtxPtr->entityMatrix.coinPtrsAllocated = 0;
    gameplayCtxPtr->entityMatrix.enemyPtrsAllocated = 0;
}

/*  Inicialização do contexto de gameplay
*/
void InitGameplayContext(GameplayContext *gameplayCtxPtr, Vector2D size, int fixedScreen, int empty)
{
    InitEntityMatrix(&gameplayCtxPtr->entityMatrix, size);
    gameplayCtxPtr->score = 0;
    gameplayCtxPtr->fixedScreen = fixedScreen;
    gameplayCtxPtr->worldSize = size;
    gameplayCtxPtr->empty = empty;
}

/*  Libera o contexto de gameplay
*/
void FreeGameplayContext(GameplayContext *gameplayCtxPtr)
{
    FreeEntityMatrix(&gameplayCtxPtr->entityMatrix);
}

void InitEntityMatrix(EntityMatrix *entityMatrixPtr, Vector2D size)
{
    entityMatrixPtr->matrix = (Entity *)malloc(size.x * size.y * sizeof(Entity));
    entityMatrixPtr->width = size.x;
    entityMatrixPtr->height = size.y;
    entityMatrixPtr->coinPtrsSize = 0;
    entityMatrixPtr->enemyPtrsSize = 0;
    entityMatrixPtr->matrixAllocated = 1;

    // Preenche a matriz com entidades vazios
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            Entity empty = CreateEmpty(CreateVector2D(j, i));
            InsertEntityOnMatrix(entityMatrixPtr, empty, (Vector2D){j, i});
        }
    }
}

/*  Insere uma entidade em uma matriz (sobescreve a entidade caso já exista uma na posição).
*/
void InsertEntityOnMatrix(EntityMatrix *entityMatrixPtr, Entity entity, Vector2D position)
{
    SetEntityInMatrix(entityMatrixPtr, position, entity);

    if (entity.type == PLAYER)
    {
        entityMatrixPtr->playerPtr = GetEntityPtrFromMatrix(entityMatrixPtr, position);
    }
    else if (entity.type == COIN)
    {
        if (entityMatrixPtr->coinPtrsSize == 0)
        {
            entityMatrixPtr->coinPtrs = (Entity **)malloc(sizeof(Entity *));
            entityMatrixPtr->coinPtrsAllocated = 1;
        }
        else
        {
            entityMatrixPtr->coinPtrs = (Entity **)realloc(entityMatrixPtr->coinPtrs,
                                                           sizeof(Entity *) * (entityMatrixPtr->coinPtrsSize + 1));
        }

        entityMatrixPtr->coinPtrsSize++;
        entityMatrixPtr->coinPtrs[entityMatrixPtr->coinPtrsSize - 1] = GetEntityPtrFromMatrix(entityMatrixPtr,
                                                                                              position);
    }
    else if (entity.type == ENEMY)
    {
        if (entityMatrixPtr->enemyPtrsSize == 0)
        {
            entityMatrixPtr->enemyPtrs = (Entity **)malloc(sizeof(Entity *));
            entityMatrixPtr->enemyPtrsAllocated = 1;
        }
        else
        {
            entityMatrixPtr->enemyPtrs = (Entity **)realloc(entityMatrixPtr->enemyPtrs,
                                                            sizeof(Entity *) * (entityMatrixPtr->enemyPtrsSize + 1));
        }

        entityMatrixPtr->enemyPtrsSize++;
        entityMatrixPtr->enemyPtrs[entityMatrixPtr->enemyPtrsSize - 1] = GetEntityPtrFromMatrix(entityMatrixPtr,
                                                                                                position);
    }
}

/*  Move um objeto na matriz caso as posições fornecidas sejam válidas. Sobescreve o objeto em
    (oldPosition.x, oldPosition.y).
*/
void MoveEntityOnMatrix(EntityMatrix *entityMatrixPtr, Vector2D oldPosition, Vector2D newPosition)
{
    Entity empty = CreateEmpty(oldPosition);
    Entity *entityPtr = GetEntityPtrFromMatrix(entityMatrixPtr, oldPosition);

    SetEntityInMatrix(entityMatrixPtr, newPosition, *entityPtr);

    if (entityPtr->type == PLAYER)
    {
        entityMatrixPtr->playerPtr = GetEntityPtrFromMatrix(entityMatrixPtr, newPosition);
    }
    else if (entityPtr->type == COIN)
    {
        for (int i = 0; i < entityMatrixPtr->coinPtrsSize; i++)
        {
            if (entityPtr == entityMatrixPtr->coinPtrs[i])
            {
                entityMatrixPtr->coinPtrs[i] = GetEntityPtrFromMatrix(entityMatrixPtr, newPosition);
            }
        }
    }
    else if (entityPtr->type == ENEMY)
    {
        for (int i = 0; i < entityMatrixPtr->enemyPtrsSize; i++)
        {
            if (entityPtr == entityMatrixPtr->enemyPtrs[i])
            {
                entityMatrixPtr->enemyPtrs[i] = GetEntityPtrFromMatrix(entityMatrixPtr, newPosition);
            }
        }
    }

    SetEntityInMatrix(entityMatrixPtr, oldPosition, empty);
}

/*  Retorna um ponteiro para a entidade na posição especificada.
*/
Entity *GetEntityPtrFromMatrix(EntityMatrix *entityMatrixPtr, Vector2D position)
{
    return &entityMatrixPtr->matrix[entityMatrixPtr->width * position.y + position.x];
}

void SetEntityInMatrix(EntityMatrix *entityMatrixPtr, Vector2D position, Entity entity)
{
    entityMatrixPtr->matrix[entityMatrixPtr->width * position.y + position.x] = entity;
}

/*  Libera a memória da matriz.
*/
void FreeEntityMatrix(EntityMatrix *entityMatrixPtr)
{
    if (entityMatrixPtr->matrixAllocated)
    {
        free(entityMatrixPtr->matrix);
        entityMatrixPtr->matrixAllocated = 0;
    }

    if (entityMatrixPtr->coinPtrsAllocated)
    {
        free(entityMatrixPtr->coinPtrs);
        entityMatrixPtr->coinPtrsAllocated = 0;
    }

    if (entityMatrixPtr->enemyPtrsAllocated)
    {
        free(entityMatrixPtr->enemyPtrs);
        entityMatrixPtr->enemyPtrsAllocated = 0;
    }
}

Vector2D GenerateSpawnPosition(EntityMatrix *entityMatrixPtr, float minDistanceFrom, Vector2D anchor)
{
    Vector2D spawn;
    Entity *entityPtrInSpawnPosition;
    float distanceFromAnchor;

    do
    {
        spawn.x = Randomf(1, entityMatrixPtr->width - 2);
        spawn.y = Randomf(1, entityMatrixPtr->height - 2);

        entityPtrInSpawnPosition = GetEntityPtrFromMatrix(entityMatrixPtr, spawn);
        distanceFromAnchor = Distance(anchor, spawn);
    } while (entityPtrInSpawnPosition->type != EMPTY || distanceFromAnchor < minDistanceFrom);

    return spawn;
}

Entity CreateEmpty(Vector2D position)
{
    Entity empty = {
        .c = {255},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x00,
        .movementAccumulator = 0.0,
        .velocityCoefficient = 1.0f,
        .direction = CreateVector2D(0, 0),
        .position = position,
        .speed = 0.0f,
        .moving = 0,
        .type = EMPTY};

    return empty;
}

Entity CreatePlayer(Vector2D position)
{
    Entity player = {
        .c = {254},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x0F,
        .movementAccumulator = 0.0,
        .velocityCoefficient = 1.0f,
        .direction = CreateVector2D(0, 0),
        .position = position,
        .speed = PLAYER_SPEED,
        .moving = 0,
        .type = PLAYER};

    return player;
}

Entity CreateCoin(Vector2D position)
{
    Entity coin = {
        .c = {45, 92, 124, 47},
        .animationFrame = 0.0f,
        .animationSpeed = ANIMATION_SPEED,
        .isAnimated = 1,
        .color = 0x0E,
        .movementAccumulator = 0.0,
        .velocityCoefficient = 1.0f,
        .direction = CreateVector2D(0, 0),
        .position = position,
        .speed = 0.0f,
        .moving = 0,
        .type = COIN};

    return coin;
}

Entity CreateEnemy(Vector2D position)
{
    Entity enemy = {
        .c = {178, 177, 176, 178},
        .animationFrame = 0.0f,
        .animationSpeed = ANIMATION_SPEED,
        .isAnimated = 1,
        .color = 0x0C,
        .movementAccumulator = 0.0,
        .velocityCoefficient = 1.0f,
        .direction = CreateVector2D(0, 0),
        .position = position,
        .speed = ENEMY_SPEED,
        .moving = 0,
        .type = ENEMY};

    return enemy;
}

Entity CreateWall(Vector2D position, char c)
{
    Entity wall = {
        .c = {c},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x07,
        .movementAccumulator = 0.0,
        .velocityCoefficient = 1.0f,
        .direction = CreateVector2D(0, 0),
        .position = position,
        .speed = 0.0f,
        .moving = 0,
        .type = WALL};

    return wall;
}

void StartUpdateThread(EventStateContext *eventStateCtxPtr,
                       GameplayContext *gameplayCtxPtr,
                       ThreadsContext *threadsCtxPtr,
                       InterfaceContext *interfaceCtxPtr,
                       TimeContext *timeCtxPtr)
{
    UpdateThreadArg *updateThreadArgPtr = malloc(sizeof(UpdateThreadArg));

    updateThreadArgPtr->eventStateCtxPtr = eventStateCtxPtr;
    updateThreadArgPtr->gameplayCtxPtr = gameplayCtxPtr;
    updateThreadArgPtr->interfaceCtxPtr = interfaceCtxPtr;
    updateThreadArgPtr->timeCtxPtr = timeCtxPtr;

    pthread_create(&threadsCtxPtr->updateThread, NULL, Update, (void *)updateThreadArgPtr);
    threadsCtxPtr->updateThreadRunning = 1;
}

void StopUpdateThread(ThreadsContext *threadsCtxPtr)
{
    if (threadsCtxPtr->updateThreadRunning)
    {
        pthread_join(threadsCtxPtr->updateThread, NULL);
        threadsCtxPtr->updateThreadRunning = 0;
    }
}

void *Update(void *updateThreadArgPtr)
{
    EventStateContext *eventStateCtxPtr = ((UpdateThreadArg *)updateThreadArgPtr)->eventStateCtxPtr;
    GameplayContext *gameplayCtxPtr = ((UpdateThreadArg *)updateThreadArgPtr)->gameplayCtxPtr;
    InterfaceContext *interfaceCtxPtr = ((UpdateThreadArg *)updateThreadArgPtr)->interfaceCtxPtr;
    TimeContext *timeCtxPtr = ((UpdateThreadArg *)updateThreadArgPtr)->timeCtxPtr;

    while (eventStateCtxPtr->state == GAMEPLAY)
    {
        StartStopWatch(&timeCtxPtr->updateFrequency, &timeCtxPtr->updateInitialTime);

        UpdateEntityBehaviour(&gameplayCtxPtr->entityMatrix, &interfaceCtxPtr->gameplay, timeCtxPtr);
        UpdateEntityPhysics(eventStateCtxPtr, gameplayCtxPtr, interfaceCtxPtr, timeCtxPtr);
    
        timeCtxPtr->updateElapsedTime = StopStopWatch(timeCtxPtr->updateFrequency, timeCtxPtr->updateInitialTime);
        SetGameplayTextd(&interfaceCtxPtr->gameplay.texts[3], 1000000.0 / timeCtxPtr->updateElapsedTime);
    }

    free((UpdateThreadArg *)updateThreadArgPtr);

    return 0;
}

/*  Atualiza cada objeto com base no seu comportamento.
*/
void UpdateEntityBehaviour(EntityMatrix *entityMatrixPtr, Interface *gameplayInterfacePtr, TimeContext *timeCtxPtr)
{
    PlayerBehaviour(entityMatrixPtr);

    for (int i = 0; i < entityMatrixPtr->enemyPtrsSize; i++)
    {
        EnemyBehaviour(entityMatrixPtr, entityMatrixPtr->enemyPtrs[i]);
    }
}

/*  Comportamento do jogador.
*/
void PlayerBehaviour(EntityMatrix *entityMatrixPtr)
{
    float runCoefficient = 1.0f;
    Vector2D direction = CreateVector2D(0, 0);

    if (GetKeyState(0x58) & 0x8000) // Tecla X
    {
        runCoefficient = 2.0f; // Dobra a velocidade na corrida
    }

    if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
    {
        direction.y = -1;
    }
    else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
    {
        direction.y = 1;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000) // Seta para a direita
    {
        direction.x = 1;
    }
    else if (GetKeyState(VK_LEFT) & 0x8000) // Seta para a esquerda
    {
        direction.x = -1;
    }

    if (direction.x != 0 || direction.y != 0)
    {
        entityMatrixPtr->playerPtr->moving = 1;
    }
    else
    {
        entityMatrixPtr->playerPtr->moving = 0;
    }

    entityMatrixPtr->playerPtr->velocityCoefficient = runCoefficient;
    entityMatrixPtr->playerPtr->direction = direction;
}

/*  Comportamento do inimigo.
*/
void EnemyBehaviour(EntityMatrix *entityMatrixPtr, Entity *enemyPtr)
{
    Vector2D position = enemyPtr->position;
    Vector2D target = entityMatrixPtr->playerPtr->position;
    Vector2D direction = CreateVector2D(0, 0);
    int blocked = 0;
    Entity *targetEntity;

    if (target.x > position.x) // Caso o jogador esteja na direita
    {
        targetEntity = GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(position.x + 1, position.y));

        if (targetEntity->type == EMPTY || targetEntity->type == PLAYER)
        {
            direction.x = 1;
        }
        else
        {
            blocked = 1;
        }
    }
    else if (target.x < position.x) // Caso o jogador esteja na esquerda
    {
        targetEntity = GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(position.x - 1, position.y));

        if (targetEntity->type == EMPTY || targetEntity->type == PLAYER)
        {
            direction.x = -1;
        }
        else
        {
            blocked = 1;
        }
    }

    if (target.y > position.y) // Caso o jogador esteja em baixo
    {
        targetEntity = GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(position.x, position.y + 1));

        if (targetEntity->type == EMPTY || targetEntity->type == PLAYER)
        {
            direction.y = 1;
        }
        else
        {
            blocked = 1;
        }
    }
    else if (target.y < position.y) // Caso o jogador esteja em cima
    {
        targetEntity = GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(position.x, position.y - 1));

        if (targetEntity->type == EMPTY || targetEntity->type == PLAYER)
        {
            direction.y = -1;
        }
        else
        {
            blocked = 1;
        }
    }

    if (blocked)
    {
        float distances[8];
        float smallestDistance = INFINITY;
        int directionIndex;
        Vector2D adjacentPositionVector;

        int distanceIndex = 0;

        for (int j = position.x - 1; j < position.x + 2; j++)
        {
            for (int k = position.y - 1; k < position.y + 2; k++)
            {
                if (j != position.x || k != position.y)
                {
                    adjacentPositionVector = CreateVector2D(j, k);

                    if (GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(j, k))->type == EMPTY)
                    {
                        distances[distanceIndex] = Distance(adjacentPositionVector, target);
                    }
                    else
                    {
                        distances[distanceIndex] = INFINITY;
                    }

                    distanceIndex++;
                }
            }
        }

        for (int j = 0; j < 8; j++)
        {
            if (smallestDistance > distances[j])
            {
                smallestDistance = distances[j];
                directionIndex = j;
            }
        }

        switch (directionIndex)
        {
        case 0:
            direction.x = -1;
            direction.y = -1;
            break;
        case 1:
            direction.x = -1;
            direction.y = 0;
            break;
        case 2:
            direction.x = -1;
            direction.y = 1;
            break;
        case 3:
            direction.x = 0;
            direction.y = -1;
            break;
        case 4:
            direction.x = 0;
            direction.y = 1;
            break;
        case 5:
            direction.x = 1;
            direction.y = -1;
            break;
        case 6:
            direction.x = 1;
            direction.y = 0;
            break;
        case 7:
            direction.x = 1;
            direction.y = 1;
            break;
        default:
            break;
        }
    }

    if (direction.x != 0 || direction.y != 0)
    {
        enemyPtr->moving = 1;
    }
    else
    {
        enemyPtr->moving = 0;
    }

    enemyPtr->direction = direction;
}

/*  Atualiza a física e verifica os eventos do gameplay.
*/
void UpdateEntityPhysics(EventStateContext *eventStateCtxPtr,
                         GameplayContext *gameplayCtxPtr,
                         InterfaceContext *interfaceCtxPtr,
                         TimeContext *timeCtxPtr)
{
    int incrementScore;
    int gameover;

    incrementScore = UpdatePlayerPhysics(gameplayCtxPtr, timeCtxPtr->updateElapsedTime);

    if (incrementScore)
    {
        gameplayCtxPtr->score++;
        SetGameplayText(&interfaceCtxPtr->gameplay.texts[5], gameplayCtxPtr->score);
    }

    for (int i = 0; i < gameplayCtxPtr->entityMatrix.enemyPtrsSize; i++)
    {
        gameover = UpdateEnemyPhysics(gameplayCtxPtr,
                                      gameplayCtxPtr->entityMatrix.enemyPtrs[i],
                                      timeCtxPtr->updateElapsedTime);

        if (gameover)
        {
            SetGameplayText(&interfaceCtxPtr->gameover.texts[2], gameplayCtxPtr->score);
            pthread_mutex_lock(&eventStateCtxPtr->eventMutex);
            eventStateCtxPtr->event = GM_GAMEOVER;
            pthread_mutex_unlock(&eventStateCtxPtr->eventMutex);
            break;
        }
    }
}

int UpdatePlayerPhysics(GameplayContext *gameplayCtxPtr, double elapsedTime)
{
    EntityMatrix *entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    Vector2D old = entityMatrixPtr->playerPtr->position;
    int coinWasPicked = 0;

    if (entityMatrixPtr->playerPtr->moving)
    {
        entityMatrixPtr->playerPtr->movementAccumulator += elapsedTime / 1000000.0;
    }

    if ((float)entityMatrixPtr->playerPtr->movementAccumulator >
        1.0f / (entityMatrixPtr->playerPtr->speed * entityMatrixPtr->playerPtr->velocityCoefficient))
    {
        entityMatrixPtr->playerPtr->movementAccumulator = 0.0;
        Vector2D target = AddVector(entityMatrixPtr->playerPtr->position, entityMatrixPtr->playerPtr->direction);
        Entity *entityPtrInTargetPosition = GetEntityPtrFromMatrix(entityMatrixPtr, target);

        for (int j = (int)target.x - 1; j < (int)target.x + 2; j++)
        {
            for (int k = (int)target.y - 1; k < (int)target.y + 2; k++)
            {
                if ((j != (int)target.x || k != (int)target.y) &&
                    (j >= 0 && j < entityMatrixPtr->width) &&
                    (k >= 0 && k < entityMatrixPtr->height))
                {

                    Vector2D adjacentPosition = CreateVector2D(j, k);
                    Entity * entityPtrInAdjacentPosition = GetEntityPtrFromMatrix(entityMatrixPtr, adjacentPosition);
                    if (entityPtrInAdjacentPosition->type == COIN)
                    {
                        Vector2D spawn;

                        spawn = GenerateSpawnPosition(entityMatrixPtr , 0.0f, CreateVector2D(0, 0));
                        entityPtrInAdjacentPosition->position = spawn;

                        // Move a moeda na matriz
                        MoveEntityOnMatrix(entityMatrixPtr, adjacentPosition, spawn);

                        spawn = GenerateSpawnPosition(entityMatrixPtr, 20.0f, target);
                        Entity enemy = CreateEnemy(spawn);
                        InsertEntityOnMatrix(entityMatrixPtr, enemy, spawn);

                        coinWasPicked = 1;
                    }
                }
            }
        }

        if (entityPtrInTargetPosition->type == EMPTY)
        {
            entityMatrixPtr->playerPtr->position = target;
            MoveEntityOnMatrix(entityMatrixPtr, old, target);
        }
    }

    return coinWasPicked;
}

int UpdateEnemyPhysics(GameplayContext *gameplayCtxPtr, Entity *enemyPtr, double elapsedTime)
{
    EntityMatrix *entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    Vector2D old = enemyPtr->position;
    int playerWasKilled = 0;

    if (enemyPtr->moving)
    {
        enemyPtr->movementAccumulator += elapsedTime / 1000000.0;
    }

    if ((float)enemyPtr->movementAccumulator > 1.0f / (enemyPtr->speed * enemyPtr->velocityCoefficient))
    {
        enemyPtr->movementAccumulator = 0.0;
        Vector2D target = AddVector(enemyPtr->position, enemyPtr->direction);
        Entity *entityPtrInTargetPosition = GetEntityPtrFromMatrix(entityMatrixPtr, target);

        if (entityPtrInTargetPosition->type == EMPTY)
        {
            enemyPtr->position = target;
            MoveEntityOnMatrix(entityMatrixPtr, old, target);
        }
        else if (entityPtrInTargetPosition->type == PLAYER)
        {
            playerWasKilled = 1;
        }
    }

    return playerWasKilled;
}

void StartRenderingThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          ConsoleContext *consoleCtxPtr,
                          InterfaceContext *interfaceCtxPtr,
                          TimeContext *timeCtxPtr)
{
    RenderThreadArg *renderThreadArgPtr = malloc(sizeof(RenderThreadArg));

    renderThreadArgPtr->statePtr = &eventStateCtxPtr->state;
    renderThreadArgPtr->entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    renderThreadArgPtr->fixedScreen = gameplayCtxPtr->fixedScreen;
    renderThreadArgPtr->consoleCtxPtr = consoleCtxPtr;
    renderThreadArgPtr->gameplayInterfacePtr = &interfaceCtxPtr->gameplay;
    renderThreadArgPtr->timeCtxPtr = timeCtxPtr;

    pthread_create(&threadsCtxPtr->renderingThread, NULL, RenderEntities, (void *)renderThreadArgPtr);
    threadsCtxPtr->renderingThreadRunning = 1;
}

void StopRenderingThread(ThreadsContext *threadsCtxPtr)
{
    if (threadsCtxPtr->renderingThreadRunning)
    {
        pthread_join(threadsCtxPtr->renderingThread, NULL);
        threadsCtxPtr->renderingThreadRunning = 0;
    }
}

/*  Rederiza as entidades.
*/
void *RenderEntities(void *renderThreadArgPtr)
{
    enum State *statePtr = ((RenderThreadArg *)renderThreadArgPtr)->statePtr;
    EntityMatrix *entityMatrixPtr = ((RenderThreadArg *)renderThreadArgPtr)->entityMatrixPtr;
    int fixedScreen = ((RenderThreadArg *)renderThreadArgPtr)->fixedScreen;
    ConsoleContext *consoleCtxPtr = ((RenderThreadArg *)renderThreadArgPtr)->consoleCtxPtr;
    Interface *gameplayInterfacePtr = ((RenderThreadArg *)renderThreadArgPtr)->gameplayInterfacePtr;
    TimeContext *timeCtxPtr = ((RenderThreadArg *)renderThreadArgPtr)->timeCtxPtr;

    Vector2Df negativeConsoleCenter = MultiplyVectorfByScalar(VectorInt2Float(consoleCtxPtr->size), -0.5);

    int validEntity;
    Vector2D position, offset, printPosition;
    Entity *entityPtr;

    while (*statePtr == GAMEPLAY)
    {
        StartStopWatch(&timeCtxPtr->renderingFrequency, &timeCtxPtr->renderingInitialTime);

        if (!fixedScreen)
        {
            offset = AddVector(entityMatrixPtr->playerPtr->position, VectorFloat2Int(negativeConsoleCenter));
        }
        else
        {
            offset = CreateVector2D(0, 0);
        }

        for (int i = 0; i < consoleCtxPtr->size.y - 1; i++)
        {
            for (int j = 0; j < consoleCtxPtr->size.x; j++)
            {
                printPosition = CreateVector2D(j, i);
                position = AddVector(printPosition, offset);

                if ((position.x >= 0 && position.x < entityMatrixPtr->width) &&
                    (position.y >= 0 && position.y < entityMatrixPtr->height))
                {
                    entityPtr = GetEntityPtrFromMatrix(entityMatrixPtr, position);
                    validEntity = entityPtr->type != EMPTY ? 1 : 0;
                }
                else
                {
                    validEntity = 0;
                }

                // Limpa o caractere caso ela esteja vazio na matriz
                if (!validEntity)
                {
                    SetCharOnPosition(consoleCtxPtr, printPosition, 32, 0x00);
                }
                else // Coloca o caractere
                {
                    int isAnimated = entityPtr->isAnimated;
                    char newChar;

                    if (isAnimated)
                    {
                        entityPtr->animationFrame += ANIMATION_SPEED *
                                                     ((float)(timeCtxPtr->renderingElapsedTime / 1000000.0));

                        if ((int)entityPtr->animationFrame > MAX_ANIM_FRAMES - 1)
                            entityPtr->animationFrame = 0.0f;

                        newChar = entityPtr->c[(int)entityPtr->animationFrame];
                    }
                    else
                    {
                        newChar = entityPtr->c[0];
                    }

                    SetCharOnPosition(consoleCtxPtr, printPosition, newChar, entityPtr->color);
                }
            }
        }

        WriteOutput(consoleCtxPtr);

        timeCtxPtr->renderingElapsedTime = StopStopWatch(timeCtxPtr->renderingFrequency,
                                                           timeCtxPtr->renderingInitialTime);

        SetGameplayTextd(&gameplayInterfacePtr->texts[1], 1000000.0 / timeCtxPtr->renderingElapsedTime);
    }

    free((RenderThreadArg *)renderThreadArgPtr);

    return 0;
}
