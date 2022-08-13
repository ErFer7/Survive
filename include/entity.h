#pragma once

#include <pthread.h>
#include <semaphore.h>

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

// Entidade
typedef struct
{
    unsigned int id;
    char c[MAX_ANIM_FRAMES];
    float animationFrame;
    float animationSpeed;
    int isAnimated;
    unsigned int color;
    float velocity[2];
    float position[2];
    float speed;
    enum EntityType type;

} Entity;

// Matriz
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

extern EntityMatrix entityMatrix;
extern unsigned int idCount;
extern pthread_t behaviourThread;
extern pthread_t physicsThread;
extern pthread_t renderingThread;
extern sem_t behaviourSemaphore;
extern sem_t physicsSemaphore;
extern int fixedScreen;

void InitEntitySemaphores();
void FreeEntitySemaphores();
void InitEntityMatrix(int width, int height);
void InsertEntityOnMatrix(Entity entity, int x, int y);
void MoveEntityOnMatrix(int x0, int y0, int x1, int y1);
Entity *GetEntityPtrFromMatrix(int x, int y);
void FreeEntityMatrix();
void GenerateSpawnPosition(float *x, float *y, float minDistanceFrom, float anchorX, float anchorY);
Entity CreateEmpty(float x, float y);
Entity CreatePlayer(float x, float y);
Entity CreateCoin(float x, float y);
Entity CreateEnemy(float x, float y);
Entity CreateWall(float x, float y, char c);
void *UpdateEntityBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(Entity *enemyPtr);
void StartBehaviourThread();
void StopBehaviourThread();
void StartPhysicsThread();
void StopPhysicsThread();
void *UpdateEntityPhysics();
void UpdatePlayerPhysics();
void UpdateEnemyPhysics(Entity *enemyPtr);
void StartRenderingThread(int fixedScreen_);
void StopRenderingThread();
void *RenderEntities();
