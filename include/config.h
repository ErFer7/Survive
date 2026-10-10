#pragma once

#include <stdint.h>

#include "utils/color.h"
#include "utils/vector.h"

static const int64_t UPDATE_FREQUENCY = 120;
static const int64_t RENDER_FREQUENCY = 60;

static const enum Color DEFAULT_BACKGROUND = HYPER_DARK_GRAY;

static const VectorU REGULAR_WORLD_SIZE = {512U, 512U};
static const VectorU SMALL_WORLD_SIZE = {128U, 128U};
static const VectorU LARGE_WORLD_SIZE = {2048U, 2048U};
static const VectorU MEGA_WORLD_SIZE = {8192U, 8192U};
static const VectorU CLASSIC_WORLD_SIZE = {120U, 30U};
