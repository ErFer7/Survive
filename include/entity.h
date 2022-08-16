#pragma once

#include <pthread.h>
#include <semaphore.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/graphics.h"

#define PLAYER_SPEED 23.5f
#define ENEMY_SPEED 13.0f
#define MAX_ANIM_FRAMES 4
#define ANIMATION_SPEED 10.0f

enum EntityType
{
    EMPTY,
    WALL,
    PLAYER,
    COIN,
    ENEMY
};

typedef struct
{
    char c[MAX_ANIM_FRAMES];
    float animationFrame;
    float animationSpeed;
    int isAnimated;
    unsigned int color;
    Vector2Df velocity;
    Vector2Df position;
    float speed;
    enum EntityType type;

} Entity;

typedef struct
{
    Entity *matrix;
    Entity *playerPtr;
    Entity **coinPtrs;
    Entity **enemyPtrs;
    int coinPtrsSize;
    int enemyPtrsSize;
    int width;
    int height;
    int allocated;

} EntityMatrix;

typedef struct
{
    EntityMatrix entityMatrix;
    int score;
    int fixedScreen;
    Vector2D worldSize;
    int empty;
} GameplayContext;

typedef struct
{
    pthread_t behaviourThread;
    pthread_t physicsThread;
    pthread_t renderingThread;
    sem_t behaviourSemaphore;
    sem_t physicsSemaphore;
} ThreadsContext;

typedef struct
{
    enum State *statePtr;
    EntityMatrix *entityMatrixPtr;
    sem_t *behaviourSemaphorePtr;
    sem_t *physicsSemaphorePtr;
} BehaviourThreadArg;

typedef struct
{
    EventStateContext *eventStateCtxPtr;
    GameplayContext *gameplayCtxPtr;
    sem_t *behaviourSemaphorePtr;
    sem_t *physicsSemaphorePtr;
} PhysicsThreadArg;

typedef struct
{
    enum State *statePtr;
    EntityMatrix *entityMatrixPtr;
    int fixedScreen;
    ConsoleContext *consoleCtxPtr;
} RenderThreadArg;

void InitGameplayContext(GameplayContext *gameplayCtxPtr, Vector2D size, int fixedScreen, int empty);
void FreeGameplayContext(GameplayContext *gameplayCtxPtr);
void InitEntitySemaphores(ThreadsContext *threadsCtxPtr);
void FreeEntitySemaphores(ThreadsContext *threadsCtxPtr);
void InitEntityMatrix(EntityMatrix *entityMatrixPtr, Vector2D size);
void InsertEntityOnMatrix(EntityMatrix *entityMatrixPtr, Entity entity, Vector2D position);
void MoveEntityOnMatrix(EntityMatrix *entityMatrixPtr, Vector2D oldPosition, Vector2D newPosition);
Entity *GetEntityPtrFromMatrix(EntityMatrix *entityMatrixPtr, Vector2D position);
void SetEntityInMatrix(EntityMatrix *entityMatrixPtr, Vector2D position, Entity entity);
void FreeEntityMatrix(EntityMatrix *entityMatrixPtr);
Vector2Df GenerateSpawnPosition(EntityMatrix *entityMatrixPtr, float minDistanceFrom, Vector2Df anchor);
Entity CreateEmpty(Vector2Df position);
Entity CreatePlayer(Vector2Df position);
Entity CreateCoin(Vector2Df position);
Entity CreateEnemy(Vector2Df position);
Entity CreateWall(Vector2Df position, char c);
void StartBehaviourThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr);
void StopBehaviourThread(ThreadsContext *threadsCtxPtr);
void *UpdateEntityBehaviour(void *behaviourThreadArgPtr);
void PlayerBehaviour(EntityMatrix *entityMatrixPtr);
void EnemyBehaviour(EntityMatrix *entityMatrixPtr, Entity *enemyPtr);
void StartPhysicsThread(EventStateContext *eventStateCtxPtr,
                        GameplayContext *gameplayCtxPtr,
                        ThreadsContext *threadsCtxPtr);
void StopPhysicsThread(ThreadsContext *threadsCtxPtr);
void *UpdateEntityPhysics(void *physicsThreadArgPtr);
void UpdatePlayerPhysics(GameplayContext *gameplayCtxPtr);
void UpdateEnemyPhysics(EventStateContext *eventStateCtxPtr, GameplayContext *gameplayCtxPtr, Entity *enemyPtr);
void StartRenderingThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          ConsoleContext *consoleCtxPtr);
void StopRenderingThread(ThreadsContext *threadsCtxPtr);
void *RenderEntities(void *renderThreadArgPtr);
