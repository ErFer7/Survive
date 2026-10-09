#include "gameplay/view.h"

#include "gameplay/world.h"
#include "utils/perlin_noise.h"
#include "utils/random.h"

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

#ifdef COOL_EFFECTS
    view->perlin_seed = (float)inclusive_random(0, 1000);
    view->shift = 0.0f;
#endif
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

#ifdef COOL_EFFECTS
    view->shift += 0.01f;
#endif

    for (int row = origin_row; row < view->position.y + half_height; row++) {
        for (int column = origin_column; column < view->position.x + half_width; column++) {
            Cell *cell = get_cell_ref_xy(world, row, column);

            if (cell != nullptr) {
                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &cell->cell);
            } else {
#ifdef COOL_EFFECTS
                // TODO: Put all constants somewhere
                float noise = perlin_noise((float)(column - ((float)origin_column / 1.25f) + view->shift) * 0.1f,
                                           (float)(row - ((float)origin_row / 1.25f) + view->shift) * 0.1f,
                                           1.75f,
                                           5,
                                           (int)view->perlin_seed);
                Cell outsize_cell = create_cell(UTF8("█"), fabsf(noise) * 0x00FFFF, VOID);

                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &outsize_cell.cell);
#else
                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &DEFAULT_CELL.cell);
#endif
            }
        }
    }
}
