#pragma once

#include <stdint.h>

#define PLAYER_SPEED 23.5f
#define ENEMY_SPEED 13.0f
#define MAX_ANIM_FRAMES 4

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
    unsigned int id;         // 2 B
    char c[MAX_ANIM_FRAMES]; // 4 B
    float animationFrame;    // 4 B
    float animationSpeed;    // 4 B
    int isAnimated;          // 1 B
    int color;               // 1 B
    float velocity[2];       // 8 B
    float position[2];       // 8 B
    float speed;             // 4 B
    enum EntityType type;    // (1 a 4) B

} Entity;

// Matriz
typedef struct
{
    Entity *matrix;      // Max: (1.625 a 1.82) MB
    Entity *playerPtr;   // (25 a 28) B
    Entity *coinPtr;     // (25 a 28) B
    unsigned int width;  // 1 B
    unsigned int height; // 1 B

} EntityMatrix;

extern EntityMatrix entityMatrix;
extern unsigned int idCount;

void EntityMatrixInit(unsigned int width, unsigned int height);
void InsertEntityOnMatrix(Entity entity, int x, int y);
void MoveEntityOnMatrix(int x0, int y0, int x1, int y1);
Entity *GetEntityPtrFromMatrix(int x, int y);
void FreeEntityMatrix();
void UpdateObjectBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(int x, int y);
void UpdatePhysics();
void RenderEntities(int tick);
