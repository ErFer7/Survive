#include <notcurses/notcurses.h>

#include "scenes/scene_context.h"

int main() {
    notcurses_options nc_options;

    nc_options.termtype = nullptr;
    nc_options.loglevel = NCLOGLEVEL_PANIC;
    nc_options.margin_t = 0;
    nc_options.margin_r = 0;
    nc_options.margin_b = 0;
    nc_options.margin_l = 0;
    nc_options.flags = 0;

    struct notcurses *nc = notcurses_init(&nc_options, stdout);
    struct ncplane *stdplane = notcurses_stdplane(nc);

    SceneContext scene_context;

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
