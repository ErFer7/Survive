#include "../include/entity.h"

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <windows.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

void InitGameplayContext(GameplayContext *gameplayCtxPtr, Vector2D size, int fixedScreen, int empty)
{
    InitEntityMatrix(&gameplayCtxPtr->entityMatrix, size);
    gameplayCtxPtr->score = 0;
    gameplayCtxPtr->fixedScreen = fixedScreen;
    gameplayCtxPtr->worldSize = size;
    gameplayCtxPtr->empty = empty;
}

void FreeGameplayContext(GameplayContext *gameplayCtxPtr)
{
    FreeEntityMatrix(&gameplayCtxPtr->entityMatrix);
}

void InitEntitySemaphores(ThreadsContext *threadsCtxPtr)
{
    sem_init(&threadsCtxPtr->behaviourSemaphore, 0, 1);
    sem_init(&threadsCtxPtr->physicsSemaphore, 0, 0);
}

void FreeEntitySemaphores(ThreadsContext *threadsCtxPtr)
{
    sem_destroy(&threadsCtxPtr->behaviourSemaphore);
    sem_destroy(&threadsCtxPtr->physicsSemaphore);
}

void InitEntityMatrix(EntityMatrix *entityMatrixPtr, Vector2D size)
{
    /* Inicializa uma matriz de entidades. A matriz é alocada na memória e possui uma largura e
       altura. Existem dois ponteiros que são úteis para manter a referência do jogador e
       moeda.
    */

    entityMatrixPtr->matrix = (Entity *)malloc(size.x * size.y * sizeof(Entity));
    entityMatrixPtr->width = size.x;
    entityMatrixPtr->height = size.y;
    entityMatrixPtr->coinPtrsSize = 0;
    entityMatrixPtr->enemyPtrsSize = 0;
    entityMatrixPtr->allocated = 1;

    // Preenche a matriz com entidades vazios
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            Entity empty = CreateEmpty((Vector2Df){i, j});
            InsertEntityOnMatrix(entityMatrixPtr, empty, (Vector2D){j, i});
        }
    }
}

void InsertEntityOnMatrix(EntityMatrix *entityMatrixPtr, Entity entity, Vector2D position)
{
    /* Insere uma entidade em uma matriz (sobescreve a entidade caso já exista uma na posição).
     */

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

void MoveEntityOnMatrix(EntityMatrix *entityMatrixPtr, Vector2D oldPosition, Vector2D newPosition)
{
    /* Move um objeto na matriz caso as posições fornecidas sejam válidas. Sobescreve o objeto em
       (oldPosition.x, oldPosition.y).
    */

    if (oldPosition.x != newPosition.x || oldPosition.y != newPosition.y)
    {
        Entity empty = CreateEmpty(VectorInt2Float(oldPosition));
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
}

Entity *GetEntityPtrFromMatrix(EntityMatrix *entityMatrixPtr, Vector2D position)
{
    /* Retorna um ponteiro para a entidade na posição especificada.
     */

    return &entityMatrixPtr->matrix[entityMatrixPtr->width * position.y + position.x];
}

void SetEntityInMatrix(EntityMatrix *entityMatrixPtr, Vector2D position, Entity entity)
{
    entityMatrixPtr->matrix[entityMatrixPtr->width * position.y + position.x] = entity;
}

void FreeEntityMatrix(EntityMatrix *entityMatrixPtr)
{
    /* Libera a memória da matriz.
     */

    if (entityMatrixPtr->allocated)
    {
        free(entityMatrixPtr->coinPtrs);
        free(entityMatrixPtr->enemyPtrs);
        free(entityMatrixPtr->matrix);
        entityMatrixPtr->enemyPtrs = NULL;
        entityMatrixPtr->matrix = NULL;
        entityMatrixPtr->width = 0;
        entityMatrixPtr->height = 0;
        entityMatrixPtr->allocated = 0;
    }
}

Vector2Df GenerateSpawnPosition(EntityMatrix *entityMatrixPtr, float minDistanceFrom, Vector2Df anchor)
{
    Vector2Df spawn;
    Entity *entityPtrInSpawnPosition;
    float distanceFromAnchor;

    do
    {
        spawn.x = Randomf(1, entityMatrixPtr->width - 2);
        spawn.y = Randomf(1, entityMatrixPtr->height - 2);

        entityPtrInSpawnPosition = GetEntityPtrFromMatrix(entityMatrixPtr, VectorFloat2Int(spawn));
        distanceFromAnchor = Distance(anchor, spawn);
    } while (entityPtrInSpawnPosition->type != EMPTY || distanceFromAnchor < minDistanceFrom);

    return spawn;
}

Entity CreateEmpty(Vector2Df position)
{
    Entity empty = {
        .c = {255},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x00,
        .velocity = CreateVector2Df(0.0f, 0.0f),
        .position = position,
        .speed = 0.0f,
        .type = EMPTY};

    return empty;
}

Entity CreatePlayer(Vector2Df position)
{
    Entity player = {
        .c = {254},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x0F,
        .velocity = CreateVector2Df(0.0f, 0.0f),
        .position = position,
        .speed = PLAYER_SPEED,
        .type = PLAYER};

    return player;
}

Entity CreateCoin(Vector2Df position)
{
    Entity coin = {
        .c = {45, 92, 124, 47},
        .animationFrame = 0.0f,
        .animationSpeed = ANIMATION_SPEED,
        .isAnimated = 1,
        .color = 0x0E,
        .velocity = CreateVector2Df(0.0f, 0.0f),
        .position = position,
        .speed = 0.0f,
        .type = COIN};

    return coin;
}

Entity CreateEnemy(Vector2Df position)
{
    Entity enemy = {
        .c = {178, 177, 176, 178},
        .animationFrame = 0.0f,
        .animationSpeed = ANIMATION_SPEED,
        .isAnimated = 1,
        .color = 0x0C,
        .velocity = CreateVector2Df(0.0f, 0.0f),
        .position = position,
        .speed = ENEMY_SPEED,
        .type = ENEMY};

    return enemy;
}

Entity CreateWall(Vector2Df position, char c)
{
    Entity wall = {
        .c = {c},
        .animationFrame = 0.0f,
        .animationSpeed = 0.0f,
        .isAnimated = 0,
        .color = 0x07,
        .velocity = CreateVector2Df(0.0f, 0.0f),
        .position = position,
        .speed = 0.0f,
        .type = WALL};

    return wall;
}

void StartBehaviourThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr)
{
    BehaviourThreadArg *behaviourThreadArgPtr = malloc(sizeof(BehaviourThreadArg));

    behaviourThreadArgPtr->statePtr = &eventStateCtxPtr->state;
    behaviourThreadArgPtr->entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    behaviourThreadArgPtr->behaviourSemaphorePtr = &threadsCtxPtr->behaviourSemaphore;
    behaviourThreadArgPtr->physicsSemaphorePtr = &threadsCtxPtr->physicsSemaphore;

    pthread_create(&threadsCtxPtr->behaviourThread, NULL, UpdateEntityBehaviour, (void *)behaviourThreadArgPtr);
}

void StopBehaviourThread(ThreadsContext *threadsCtxPtr)
{
    pthread_join(threadsCtxPtr->behaviourThread, NULL);
}

void *UpdateEntityBehaviour(void *behaviourThreadArgPtr)
{
    /* Atualiza cada objeto com base no seu comportamento.
     */

    enum State *statePtr = ((BehaviourThreadArg *)behaviourThreadArgPtr)->statePtr;
    EntityMatrix *entityMatrixPtr = ((BehaviourThreadArg *)behaviourThreadArgPtr)->entityMatrixPtr;
    sem_t *behaviourSemaphorePtr = ((BehaviourThreadArg *)behaviourThreadArgPtr)->behaviourSemaphorePtr;
    sem_t *physicsSemaphorePtr = ((BehaviourThreadArg *)behaviourThreadArgPtr)->physicsSemaphorePtr;

    while (*statePtr == GAMEPLAY)
    {
        StartChronometer(&behaviourFrequency, &behaviourInitialTime);

        if (sem_trywait(behaviourSemaphorePtr))
        {
            PlayerBehaviour(entityMatrixPtr);

            for (int i = 0; i < entityMatrixPtr->enemyPtrsSize; i++)
            {
                EnemyBehaviour(entityMatrixPtr, entityMatrixPtr->enemyPtrs[i]);
            }
            sem_post(physicsSemaphorePtr);

            behaviourElapsedTime = StopChronometer(behaviourFrequency, behaviourInitialTime, &behaviourFinalTime);

            sprintf(gameplay.texts[1].content, "BLT: %08.3f ms", (float)behaviourElapsedTime);
            gameplay.texts[1].update = 1;
        }
    }

    free((BehaviourThreadArg *)behaviourThreadArgPtr);

    return 0;
}

void PlayerBehaviour(EntityMatrix *entityMatrixPtr)
{
    /* Comportamento do jogador.
     */

    float runCoefficient = 1.0f; // Coeficiente de corrida

    if (GetKeyState(0x58) & 0x8000) // Tecla X
    {
        runCoefficient = 2.0f; // Dobra a velocidade na corrida
    }

    if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
    {
        // Adiciona velocidade para cima
        entityMatrixPtr->playerPtr->velocity.y = -entityMatrixPtr->playerPtr->speed * runCoefficient;
    }
    else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
    {
        // Adiciona velocidade para baixo
        entityMatrixPtr->playerPtr->velocity.y = entityMatrixPtr->playerPtr->speed * runCoefficient;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000) // Seta para a direita
    {
        // Adiciona velocidade para direita
        entityMatrixPtr->playerPtr->velocity.x = entityMatrixPtr->playerPtr->speed * runCoefficient;
    }
    else if (GetKeyState(VK_LEFT) & 0x8000) // Seta para a esquerda
    {
        // Adiciona velocidade para esquerda
        entityMatrixPtr->playerPtr->velocity.x = -entityMatrixPtr->playerPtr->speed * runCoefficient;
    }

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (entityMatrixPtr->playerPtr->velocity.x != 0.0f && entityMatrixPtr->playerPtr->velocity.y != 0.0f)
    {
        // Obtém a posição esperada
        Vector2Df vt = MultiplyVectorfByScalar(entityMatrixPtr->playerPtr->velocity, 1.0f / (float)tick);
        Vector2D expectedPosition = VectorFloat2Int(AddVectorf(entityMatrixPtr->playerPtr->position, vt));

        // Corrige a velocidade vertical
        if (expectedPosition.x == (int)entityMatrixPtr->playerPtr->position.x &&
            expectedPosition.y != (int)entityMatrixPtr->playerPtr->position.y)
            entityMatrixPtr->playerPtr->velocity.y = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPosition.x != (int)entityMatrixPtr->playerPtr->position.x &&
            expectedPosition.y == (int)entityMatrixPtr->playerPtr->position.y)
            entityMatrixPtr->playerPtr->velocity.x = 0.0f;
    }
}

void EnemyBehaviour(EntityMatrix *entityMatrixPtr, Entity *enemyPtr)
{
    /* Comportamento do inimigo.
     */

    Vector2D position = VectorFloat2Int(enemyPtr->position);
    Vector2D target = VectorFloat2Int(entityMatrixPtr->playerPtr->position);
    Vector2Df direction = CreateVector2Df(0.0f, 0.0f);
    int blocked = 0;
    Entity *targetEntity;

    if (target.x > position.x) // Caso o jogador esteja na direita
    {
        targetEntity = GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(position.x + 1, position.y));

        if (targetEntity->type == EMPTY || targetEntity->type == PLAYER)
        {
            direction.x = 1.0f;
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
            direction.x = -1.0f;
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
            direction.y = 1.0f;
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
            direction.y = -1.0f;
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

        int distanceIndex = 0;

        for (int j = position.x - 1; j < position.x + 2; j++)
        {
            for (int k = position.y - 1; k < position.y + 2; k++)
            {
                if (j != position.x || k != position.y)
                {
                    if (GetEntityPtrFromMatrix(entityMatrixPtr, CreateVector2D(j, k))->type == EMPTY)
                    {
                        distances[distanceIndex] = sqrtf(powf((j - target.x), 2.0f) + powf((k - target.y), 2.0f));
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
            direction.x = -1.0f;
            direction.y = -1.0f;
            break;
        case 1:
            direction.x = -1.0f;
            direction.y = 0.0f;
            break;
        case 2:
            direction.x = -1.0f;
            direction.y = 1.0f;
            break;
        case 3:
            direction.x = 0.0f;
            direction.y = -1.0f;
            break;
        case 4:
            direction.x = 0.0f;
            direction.y = 1.0f;
            break;
        case 5:
            direction.x = 1.0f;
            direction.y = -1.0f;
            break;
        case 6:
            direction.x = 1.0f;
            direction.y = 0.0f;
            break;
        case 7:
            direction.x = 1.0f;
            direction.y = 1.0f;
            break;
        default:
            break;
        }
    }

    enemyPtr->velocity = MultiplyVectorfByScalar(direction, enemyPtr->speed);

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes)

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (enemyPtr->velocity.x != 0.0f && enemyPtr->velocity.y != 0.0f)
    {
        Vector2Df vt = MultiplyVectorfByScalar(enemyPtr->velocity, 1.0f / (float)tick);
        Vector2D expectedPosition = VectorFloat2Int(AddVectorf(enemyPtr->position, vt));

        // Corrige a velocidade vertical
        if (expectedPosition.x == (int)enemyPtr->position.x &&
            expectedPosition.y != (int)enemyPtr->position.y)
            enemyPtr->velocity.y = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPosition.x != (int)enemyPtr->position.x &&
            expectedPosition.y == (int)enemyPtr->position.y)
            enemyPtr->velocity.x = 0.0f;
    }
}

void StartPhysicsThread(EventStateContext *eventStateCtxPtr,
                        GameplayContext *gameplayCtxPtr,
                        ThreadsContext *threadsCtxPtr)
{
    PhysicsThreadArg *physicsThreadArgPtr = malloc(sizeof(PhysicsThreadArg));

    physicsThreadArgPtr->eventStateCtxPtr = eventStateCtxPtr;
    physicsThreadArgPtr->gameplayCtxPtr = gameplayCtxPtr;
    physicsThreadArgPtr->behaviourSemaphorePtr = &threadsCtxPtr->behaviourSemaphore;
    physicsThreadArgPtr->physicsSemaphorePtr = &threadsCtxPtr->physicsSemaphore;

    pthread_create(&threadsCtxPtr->physicsThread, NULL, UpdateEntityPhysics, (void *)physicsThreadArgPtr);
}

void StopPhysicsThread(ThreadsContext *threadsCtxPtr)
{
    pthread_join(threadsCtxPtr->physicsThread, NULL);
}

void *UpdateEntityPhysics(void *physicsThreadArgPtr)
{
    /* Atualiza a física e verifica os eventos do gameplay.
     */

    EventStateContext *eventStateCtxPtr = ((PhysicsThreadArg *)physicsThreadArgPtr)->eventStateCtxPtr;
    GameplayContext *gameplayCtxPtr = ((PhysicsThreadArg *)physicsThreadArgPtr)->gameplayCtxPtr;
    sem_t *behaviourSemaphorePtr = ((BehaviourThreadArg *)physicsThreadArgPtr)->behaviourSemaphorePtr;
    sem_t *physicsSemaphorePtr = ((BehaviourThreadArg *)physicsThreadArgPtr)->physicsSemaphorePtr;

    while (eventStateCtxPtr->state == GAMEPLAY)
    {
        StartChronometer(&physicsFrequency, &physicsInitialTime);

        sem_wait(physicsSemaphorePtr);
        UpdatePlayerPhysics(gameplayCtxPtr);

        for (int i = 0; i < gameplayCtxPtr->entityMatrix.enemyPtrsSize; i++)
        {
            UpdateEnemyPhysics(eventStateCtxPtr, gameplayCtxPtr, gameplayCtxPtr->entityMatrix.enemyPtrs[i]);
        }

        sem_post(behaviourSemaphorePtr);

        physicsElapsedTime = StopChronometer(physicsFrequency, physicsInitialTime, &physicsFinalTime);
        physicsElapsedTime = Tick(physicsElapsedTime);

        sprintf(gameplay.texts[2].content, "PLT: %08.3f ms", (float)physicsElapsedTime);
        gameplay.texts[2].update = 1;
    }

    free((PhysicsThreadArg *)physicsThreadArgPtr);

    return 0;
}

void UpdatePlayerPhysics(GameplayContext *gameplayCtxPtr)
{
    EntityMatrix *entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    Vector2D old = VectorFloat2Int(entityMatrixPtr->playerPtr->position);
    Vector2Df vt = MultiplyVectorfByScalar(entityMatrixPtr->playerPtr->velocity, (float)physicsElapsedTime / 1000.0f);
    Vector2Df target = AddVectorf(entityMatrixPtr->playerPtr->position, vt);

    // Redefine a velocidade
    entityMatrixPtr->playerPtr->velocity = CreateVector2Df(0.0f, 0.0f);

    // Ponteiro do objeto na posição alvo
    Entity *entityPtrInTargetPosition = GetEntityPtrFromMatrix(entityMatrixPtr, VectorFloat2Int(target));

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
                    gameplayCtxPtr->score++;

                    // Atualiza o texto da pontuação
                    sprintf(gameplay.texts[3].content, "Score: %010d", gameplayCtxPtr->score);
                    gameplay.texts[3].update = 1;

                    Vector2Df spawn;

                    spawn = GenerateSpawnPosition(entityMatrixPtr , 0.0f, CreateVector2Df(0.0f, 0.0f));
                    entityPtrInAdjacentPosition->position = spawn;

                    // Move a moeda na matriz
                    MoveEntityOnMatrix(entityMatrixPtr, adjacentPosition, VectorFloat2Int(spawn));

                    spawn = GenerateSpawnPosition(entityMatrixPtr, 20.0f, target);
                    Entity enemy = CreateEnemy(spawn);
                    InsertEntityOnMatrix(entityMatrixPtr, enemy, VectorFloat2Int(spawn));
                }
            }
        }
    }

    /* Move o jogador caso o objeto na posição alvo esteja vazio ou seja o próprio
        jogador
    */
    if (entityPtrInTargetPosition->type == EMPTY || entityPtrInTargetPosition == entityMatrixPtr->playerPtr)
    {
        // Define a nova posição
        entityMatrixPtr->playerPtr->position = target;

        // Move o jogador na matriz
        MoveEntityOnMatrix(entityMatrixPtr, old, VectorFloat2Int(target));
    }
}

void UpdateEnemyPhysics(EventStateContext *eventStateCtxPtr, GameplayContext *gameplayCtxPtr, Entity *enemyPtr)
{
    EntityMatrix *entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    Vector2D old = VectorFloat2Int(enemyPtr->position);
    Vector2Df vt = MultiplyVectorfByScalar(enemyPtr->velocity, (float)physicsElapsedTime / 1000.0f);
    Vector2Df target = AddVectorf(enemyPtr->position, vt);

    // Redefine a velocidade
    enemyPtr->velocity = CreateVector2Df(0.0f, 0.0f);

    // Ponteiro do objeto na posição alvo
    Entity *entityPtrInTargetPosition = GetEntityPtrFromMatrix(entityMatrixPtr, VectorFloat2Int(target));

    // Se move caso o a posição alvo não tenha um objeto ou tenha o próprio inimigo
    if (entityPtrInTargetPosition->type == EMPTY || entityPtrInTargetPosition == enemyPtr)
    {
        enemyPtr->position = target;

        MoveEntityOnMatrix(entityMatrixPtr,
                           old,
                           VectorFloat2Int(target));
    }
    // Caso a posição tenha o jogador
    else if (entityPtrInTargetPosition->type == PLAYER)
    {
        pthread_mutex_lock(&eventStateCtxPtr->eventMutex);
        eventStateCtxPtr->event = GM_GAMEOVER;
        pthread_mutex_unlock(&eventStateCtxPtr->eventMutex);
        sprintf(gameover.texts[1].content, "Score: %010d", gameplayCtxPtr->score);
    }
}

void StartRenderingThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          ConsoleContext *consoleCtxPtr)
{
    RenderThreadArg *renderThreadArgPtr = malloc(sizeof(RenderThreadArg));

    renderThreadArgPtr->statePtr = &eventStateCtxPtr->state;
    renderThreadArgPtr->entityMatrixPtr = &gameplayCtxPtr->entityMatrix;
    renderThreadArgPtr->fixedScreen = gameplayCtxPtr->fixedScreen;
    renderThreadArgPtr->consoleCtxPtr = consoleCtxPtr;

    pthread_create(&threadsCtxPtr->renderingThread, NULL, RenderEntities, (void *)renderThreadArgPtr);
}

void StopRenderingThread(ThreadsContext *threadsCtxPtr)
{
    pthread_join(threadsCtxPtr->renderingThread, NULL);
}

void *RenderEntities(void *renderThreadArgPtr)
{
    /* Rederiza as entidades.
     */

    enum State *statePtr = ((RenderThreadArg *)renderThreadArgPtr)->statePtr;
    EntityMatrix *entityMatrixPtr = ((RenderThreadArg *)renderThreadArgPtr)->entityMatrixPtr;
    int fixedScreen = ((RenderThreadArg *)renderThreadArgPtr)->fixedScreen;
    ConsoleContext *consoleCtxPtr = ((RenderThreadArg *)renderThreadArgPtr)->consoleCtxPtr;

    Vector2Df negativeConsoleCenter = MultiplyVectorfByScalar(VectorInt2Float(consoleCtxPtr->size), -0.5);

    int validEntity;
    Vector2D position, offset, printPosition;
    Entity *entityPtr;

    while (*statePtr == GAMEPLAY)
    {
        StartChronometer(&renderingFrequency, &renderingInitialTime);

        if (!fixedScreen)
        {
            offset = VectorFloat2Int(AddVectorf(entityMatrixPtr->playerPtr->position, negativeConsoleCenter));
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
                        entityPtr->animationFrame += ANIMATION_SPEED * ((float)renderingElapsedTime / 1000.0f);

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

        renderingElapsedTime = StopChronometer(renderingFrequency, renderingInitialTime, &renderingFinalTime);

        sprintf(gameplay.texts[0].content, "FPS: %08.3f", 1000.0f / (float)renderingElapsedTime);
        gameplay.texts[0].update = 1;
    }

    free((RenderThreadArg *)renderThreadArgPtr);

    return 0;
}
