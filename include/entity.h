#pragma once

#include <pthread.h>
#include <semaphore.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

#define PLAYER_SPEED 30.0f
#define ENEMY_SPEED 15.0f
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
    float velocityAccumulator;
    float velocityCoefficient;
    Vector2D direction;
    Vector2D position;
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
    int matrixAllocated;
    int coinPtrsAllocated;
    int enemyPtrsAllocated;

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
    Interface *gameplayInterfacePtr;
    TimeContext *timeCtxPtr;
} BehaviourThreadArg;

typedef struct
{
    EventStateContext *eventStateCtxPtr;
    GameplayContext *gameplayCtxPtr;
    sem_t *behaviourSemaphorePtr;
    sem_t *physicsSemaphorePtr;
    InterfaceContext *interfaceCtxPtr;
    TimeContext *timeCtxPtr;
} PhysicsThreadArg;

typedef struct
{
    enum State *statePtr;
    EntityMatrix *entityMatrixPtr;
    int fixedScreen;
    ConsoleContext *consoleCtxPtr;
    Interface *gameplayInterfacePtr;
    TimeContext *timeCtxPtr;
} RenderThreadArg;

void PreInitGameplayContext(GameplayContext *gameplayCtxPtr);
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
Vector2D GenerateSpawnPosition(EntityMatrix *entityMatrixPtr, float minDistanceFrom, Vector2D anchor);
Entity CreateEmpty(Vector2D position);
Entity CreatePlayer(Vector2D position);
Entity CreateCoin(Vector2D position);
Entity CreateEnemy(Vector2D position);
Entity CreateWall(Vector2D position, char c);
void StartBehaviourThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          InterfaceContext *interfaceCtxPtr,
                          TimeContext *timeCtxPtr);
void StopBehaviourThread(ThreadsContext *threadsCtxPtr);
void *UpdateEntityBehaviour(void *behaviourThreadArgPtr);
void PlayerBehaviour(EntityMatrix *entityMatrixPtr);
void EnemyBehaviour(EntityMatrix *entityMatrixPtr, Entity *enemyPtr);
void StartPhysicsThread(EventStateContext *eventStateCtxPtr,
                        GameplayContext *gameplayCtxPtr,
                        ThreadsContext *threadsCtxPtr,
                        InterfaceContext *interfaceCtxPtr,
                        TimeContext *timeCtxPtr);
void StopPhysicsThread(ThreadsContext *threadsCtxPtr);
void *UpdateEntityPhysics(void *physicsThreadArgPtr);
void UpdatePlayerPhysics(GameplayContext *gameplayCtxPtr, Interface *gameplayInterfacePtr, float elapsedTime);
void UpdateEnemyPhysics(EventStateContext *eventStateCtxPtr,
                        GameplayContext *gameplayCtxPtr,
                        Interface *gameoverInterfacePtr,
                        Entity *enemyPtr,
                        float elapsedTime);
void StartRenderingThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          ConsoleContext *consoleCtxPtr,
                          InterfaceContext *interfaceCtxPtr,
                          TimeContext *timeCtxPtrs);
void StopRenderingThread(ThreadsContext *threadsCtxPtr);
void *RenderEntities(void *renderThreadArgPtr);
