#include <notcurses/notcurses.h>

#include "scenes/scene_context.h"

int main() {
    SceneContext scene_context;

    notcurses_options nc_options;
    memset(&nc_options, 0, sizeof(notcurses_options));

    struct notcurses *nc = notcurses_init(&nc_options, stdout);
    struct ncplane *stdplane = notcurses_stdplane(nc);

    init_scene_context(&scene_context, stdplane);

    // TODO: Fix the semantics of update and render, drawing the text should be a rendering thing
    while (!is_exiting(&scene_context)) {
        update_scene(&scene_context);
        notcurses_render(nc);
    }

    free_scene_context(&scene_context);

    notcurses_stop(nc);

    return 0;
}
