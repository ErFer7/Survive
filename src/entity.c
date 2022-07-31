#include "../include/entity.h"

#include <stdint.h>
#include <stdlib.h>

void EntityMatrixInit(EntityMatrix *entityMatrix, unsigned int width, unsigned int height)
{
    /* Inicializa uma matriz de entidades. A matriz é alocada na memória e possui uma largura e
       altura. Existem dois ponteiros que são úteis para manter a referência do jogador e
       moeda.
    */

    entityMatrix->matrix = (Entity *)malloc(width * height * sizeof(Entity));
    entityMatrix->width = width;
    entityMatrix->height = height;

    // Preenche a matriz com entidades vazios
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            Entity empty = {0,
                            {255},
                            0.0f,
                            0.0f,
                            0,
                            0x00,
                            {0.0f, 0.0f},
                            {(float)i, (float)j},
                            0.0f,
                            EMPTY};

            InsertEntityOnMatrix(entityMatrix, empty, j, i);
        }
    }
}

void InsertEntityOnMatrix(EntityMatrix *entityMatrix, Entity entity, unsigned int x, unsigned int y)
{
    /* Insere uma entidade em uma matriz (sobescreve a entidade caso já exista uma na posição).
     */

    entityMatrix->matrix[entityMatrix->width * y + x] = entity;

    if (entity.type == PLAYER)
        entityMatrix->playerPtr = &entityMatrix->matrix[entityMatrix->width * y + x];
    else if (entity.type == COIN)
        entityMatrix->coinPtr = &entityMatrix->matrix[entityMatrix->width * y + x];
}

void MoveEntityOnMatrix(EntityMatrix *entityMatrix, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    /* Move um objeto na matriz caso as posições fornecidas sejam válidas. Sobescreve o objeto em
       (x1, y1).
    */

    if (x0 != x1 || y0 != y1)
    {
        Entity empty = {0, {255}, 0.0f, 0.0f, 0, 0, {0.0f, 0.0f}, {(float)x0, (float)y0}, 0.0f, EMPTY};
        Entity object = entityMatrix->matrix[entityMatrix->width * y0 + x0];

        entityMatrix->matrix[entityMatrix->width * y1 + x1] = object;
        entityMatrix->matrix[entityMatrix->width * y0 + x0] = empty;

        if (object.type == PLAYER)
            entityMatrix->playerPtr = &entityMatrix->matrix[entityMatrix->width * y1 + x1];
        else if (object.type == COIN)
            entityMatrix->coinPtr = &entityMatrix->matrix[entityMatrix->width * y1 + x1];
    }
}

Entity *GetEntityPtrFromMatrix(EntityMatrix *entityMatrix, unsigned int x, unsigned int y)
{
    /* Retorna um ponteiro para a entidade na posição especificada.
     */

    return &entityMatrix->matrix[entityMatrix->width * y + x];
}

void FreeEntityMatrix(EntityMatrix *entityMatrix)
{
    /* Libera a memória da matriz.
     */

    free(entityMatrix->matrix);
    entityMatrix->matrix = NULL;
    entityMatrix->width = 0;
    entityMatrix->height = 0;
}
