#include "gameplay/view.h"

#include "gameplay/world.h"
#include "utils/perlin_noise.h"
#include "utils/random.h"

void init_view(View *view,
               VectorU initial_position,
               VectorU size,
               VectorU parent_size,
               enum Color background_color,
               struct ncplane *parent_plane) {
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

    uint64_t channels = 0;
    byte red;
    byte green;
    byte blue;

    break_into_parts(background_color, &red, &green, &blue);

    ncchannels_set_bg_rgb8(&channels, (uint32_t)red, (uint32_t)green, (uint32_t)blue);
    ncchannels_set_bg_alpha(&channels, NCALPHA_OPAQUE);
    ncplane_set_base(view->plane, " ", 0, channels);
    ncplane_erase(view->plane);

#ifdef COOL_EFFECTS
    view->perlin_seed = (float)inclusive_random(0, 1000);
    view->shift = 0.0f;
#endif
}

// PERFORMANCE: Optimize this. The effects could be generated with a matrix with 3 moving pointers
void draw_world_on_view(View *view, World *world) {
    uint32_t rows = 0;
    uint32_t columns = 0;

    ncplane_dim_yx(view->plane, &rows, &columns);

    int32_t half_height = (int32_t)rows / 2;
    int32_t half_width = (int32_t)columns / 2;

    int32_t origin_row = view->position.y - half_height;
    int32_t origin_column = view->position.x - half_width;

#ifdef COOL_EFFECTS
    view->shift += 0.01f;
#endif

    for (int32_t row = origin_row; row < (int32_t)view->position.y + half_height; row++) {
        for (int32_t column = origin_column; column < (int32_t)view->position.x + half_width; column++) {
            Cell *cell = get_cell_ref_xy(world, (uint32_t)row, (uint32_t)column);

            if (cell != nullptr) {
                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &cell->cell);
            } else {
#ifdef COOL_EFFECTS
                // TODO: Put all constants somewhere
                float noise = perlin_noise((float)(column - ((float)origin_column / 1.25f)) * 0.1f,
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
