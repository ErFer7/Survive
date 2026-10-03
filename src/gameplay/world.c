#include "gameplay/world.h"

#include "stdlib.h"

void init_world(World *world, Vector size) { world->matrix = malloc(sizeof(Cell) * size.x * size.y); }

void free_world(World *world) { free(world->matrix); }
