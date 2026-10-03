#pragma once

#include "cell.h"
#include "utils/vector.h"

struct World {
    Cell *matrix;
    Cell *player;
    Cell *coins;
    Cell *enemies;
};

void init_world(World *world, Vector size);
void free_world(World *world);
