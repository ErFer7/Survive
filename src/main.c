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

// TODO: Handle resize
int main() {
    SceneContext scene_context;

    init_scene_context(&scene_context);
    run(&scene_context);
    free_scene_context(&scene_context);

    return 0;
}
