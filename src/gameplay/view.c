#include "gameplay/view.h"

#include "gameplay/world.h"

void init_view(View *view, Vector initial_position, Vector size, Vector parent_size, struct ncplane *parent_plane) {
    view->position = initial_position;

    const struct ncplane_options plane_options = {(parent_size.y - size.y) / 2,
                                                  (parent_size.x - size.x) / 2,
                                                  size.y,
                                                  size.x,
                                                  nullptr,
                                                  nullptr,
                                                  nullptr,
                                                  NCPLANE_OPTION_FIXED,
                                                  0,
                                                  0};

    memcpy(&view->plane_options, &plane_options, sizeof(ncplane_options));

    view->plane = ncplane_create(parent_plane, &view->plane_options);
    ncplane_reparent(view->plane, parent_plane);
}

// PERFORMANCE: Optimize this
void draw_world_on_view(View *view, World *world) {
    unsigned int rows = 0;
    unsigned int columns = 0;

    ncplane_dim_yx(view->plane, &rows, &columns);

    int half_height = (int)rows / 2;
    int half_width = (int)columns / 2;

    int origin_row = view->position.y - half_height;
    int origin_column = view->position.x - half_width;

    for (int row = origin_row; row < view->position.y + half_height; row++) {
        for (int column = origin_column; column < view->position.x + half_width; column++) {
            Cell *cell = get_cell_ref_xy(world, row, column);

            ncplane_set_fg_rgb(view->plane, cell->color);
            ncplane_putchar_yx(view->plane, row - origin_row, column - origin_column, cell->character);
            ncplane_set_bg_default(view->plane);
        }
    }
}
