#pragma once

#include "utils/vector.h"

enum Alignment { TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT };

VectorU aligned_position(Vector position, VectorU size, VectorU parent_size, enum Alignment alignment);
