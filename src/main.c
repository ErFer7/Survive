#include "scenes/scene_context.h"

/*
    ███████╗██╗   ██╗██████╗ ██╗   ██╗██╗██╗   ██╗███████╗
    ██╔════╝██║   ██║██╔══██╗██║   ██║██║██║   ██║██╔════╝
    ███████╗██║   ██║██████╔╝██║   ██║██║██║   ██║█████╗
    ╚════██║██║   ██║██╔══██╗╚██╗ ██╔╝██║╚██╗ ██╔╝██╔══╝
    ███████║╚██████╔╝██║  ██║ ╚████╔╝ ██║ ╚████╔╝ ███████╗
    ╚══════╝ ╚═════╝ ╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═══╝  ╚══════╝

    A terminal game made in C.
*/

// TODO: Analyze the design of create_* and init_* and also simplify the names
int main() {
    SceneContext scene_context;

    init_scene_context(&scene_context);
    run(&scene_context);
    free_scene_context(&scene_context);

    return 0;
}
