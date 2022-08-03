#pragma once

#include <stdint.h>

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
    int coinPtrsSize;
    unsigned int width;
    unsigned int height;

} EntityMatrix;

extern EntityMatrix entityMatrix;
extern unsigned int idCount;
extern unsigned int simulationAreaWidth, simulationAreaHeight;

void SetSimulationAreaSize(unsigned int width, unsigned int height);
void InitEntityMatrix(unsigned int width, unsigned int height);
void InsertEntityOnMatrix(Entity entity, int x, int y);
void MoveEntityOnMatrix(int x0, int y0, int x1, int y1);
Entity *GetEntityPtrFromMatrix(int x, int y);
void FreeEntityMatrix();
void UpdateObjectBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(int x, int y);
void UpdatePhysics();
void RenderEntities(int tick);
