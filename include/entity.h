#pragma once

#include <pthread.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"

#define PLAYER_SPEED 20.0f
#define ENEMY_SPEED 10.0f
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
    double movementAccumulator;
    float velocityCoefficient;
    Vector2D direction;
    Vector2D position;
    float speed;
    int moving;
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
    pthread_t renderingThread;
    pthread_t updateThread;
    int updateThreadRunning;
    int renderingThreadRunning;
} ThreadsContext;

typedef struct
{
    EventStateContext *eventStateCtxPtr;
    GameplayContext *gameplayCtxPtr;
    InterfaceContext *interfaceCtxPtr;
    TimeContext *timeCtxPtr;
} UpdateThreadArg;

typedef struct
{
    enum State *statePtr;
    EntityMatrix *entityMatrixPtr;
    int fixedScreen;
    ConsoleContext *consoleCtxPtr;
    Interface *gameplayInterfacePtr;
    TimeContext *timeCtxPtr;
} RenderThreadArg;

void PreInitThreadsContext(ThreadsContext *threadsCtxPtr);
void PreInitGameplayContext(GameplayContext *gameplayCtxPtr);
void InitGameplayContext(GameplayContext *gameplayCtxPtr, Vector2D size, int fixedScreen, int empty);
void FreeGameplayContext(GameplayContext *gameplayCtxPtr);
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
void StartUpdateThread(EventStateContext *eventStateCtxPtr,
                       GameplayContext *gameplayCtxPtr,
                       ThreadsContext *threadsCtxPtr,
                       InterfaceContext *interfaceCtxPtr,
                       TimeContext *timeCtxPtr);
void StopUpdateThread(ThreadsContext *threadsCtxPtr);
void *Update(void *updateThreadArgPtr);
void UpdateEntityBehaviour(EntityMatrix *entityMatrixPtr, Interface *gameplayInterfacePtr, TimeContext *timeCtxPtr);
void PlayerBehaviour(EntityMatrix *entityMatrixPtr);
void EnemyBehaviour(EntityMatrix *entityMatrixPtr, Entity *enemyPtr);
void UpdateEntityPhysics(EventStateContext *eventStateCtxPtr,
                         GameplayContext *gameplayCtxPtr,
                         InterfaceContext *interfaceCtxPtr,
                         TimeContext *timeCtxPtr);
int UpdatePlayerPhysics(GameplayContext *gameplayCtxPtr, double elapsedTime);
int UpdateEnemyPhysics(GameplayContext *gameplayCtxPtr, Entity *enemyPtr, double elapsedTime);
void StartRenderingThread(EventStateContext *eventStateCtxPtr,
                          GameplayContext *gameplayCtxPtr,
                          ThreadsContext *threadsCtxPtr,
                          ConsoleContext *consoleCtxPtr,
                          InterfaceContext *interfaceCtxPtr,
                          TimeContext *timeCtxPtrs);
void StopRenderingThread(ThreadsContext *threadsCtxPtr);
void *RenderEntities(void *renderThreadArgPtr);
