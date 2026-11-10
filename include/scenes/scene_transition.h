#pragma once

#include "types.h"

#define TRANSITION_ARGS_SIZE 16

struct SceneTransition {
    SceneContext *scene_context;
    Scene *next_scene;
    byte enter_args[TRANSITION_ARGS_SIZE];
    byte exit_args[TRANSITION_ARGS_SIZE];
};
