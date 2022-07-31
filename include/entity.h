#pragma once

#include <stdint.h>

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
    uint8_t color;           // 1 B
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

EntityMatrix entityMatrix;

void EntityMatrixInit(EntityMatrix *entityMatrix, unsigned int width, unsigned int height);
void InsertEntityOnMatrix(EntityMatrix *entityMatrix, Entity entity, unsigned int x, unsigned int y);
void MoveEntityOnMatrix(EntityMatrix *entityMatrix, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
Entity *GetEntityPtrFromMatrix(EntityMatrix *entityMatrix, unsigned int x, unsigned int y);
void FreeEntityMatrix(EntityMatrix *entityMatrix);
