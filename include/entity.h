#pragma once

#include <stdint.h>
#include <pthread.h>

#define PLAYER_SPEED 23.5f
#define ENEMY_SPEED 13.0f
#define MAX_ANIM_FRAMES 4
#define MAX_COINS 73

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
    int color;
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
    Entity *coinPtrs[MAX_COINS];
    Entity **enemyPtrs;
    int coinPtrsSize;
    int enemyPtrsSize;
    int width;
    int height;
    int allocated;

} EntityMatrix;

extern EntityMatrix entityMatrix;
extern unsigned int idCount;
extern pthread_t renderingThread;

void InitEntityMatrix(int width, int height);
void InsertEntityOnMatrix(Entity entity, int x, int y);
void MoveEntityOnMatrix(int x0, int y0, int x1, int y1);
Entity *GetEntityPtrFromMatrix(int x, int y);
void FreeEntityMatrix();
void UpdateEntityBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(Entity *enemyPtr);
void UpdateEntityPhysics();
void UpdatePlayerPhysics();
void UpdateEnemyPhysics(Entity *enemyPtr);
void StartRenderingThread();
void StopRenderingThread();
void *RenderEntities();
