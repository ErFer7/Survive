#pragma once

#include "../utils/vector.h"
#include "types.h"

enum Alignment { TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT };

Vector aligned_position(Vector position, Vector size, Vector parent_size, enum Alignment alignment);
