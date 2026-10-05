#include "scenes/scene_context.h"

// TODO: Change all primitive types to sized types

int main() {
    SceneContext scene_context;

    init_scene_context(&scene_context);
    run(&scene_context);
    free_scene_context(&scene_context);

    return 0;
}
