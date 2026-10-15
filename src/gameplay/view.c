#include "gameplay/view.h"

#include <math.h>
#include <stdint.h>

#include "config.h"
#include "gameplay/world.h"
#include "utils/perlin_noise.h"

void init_view(View *view,
               Vector initial_position,
               VectorU size,
               VectorU parent_size,
               enum Color background_color,
               struct ncplane *parent_plane,
               pthread_mutex_t *world_mutex) {
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
    view->parent_plane = parent_plane;

    uint64_t channels = 0;
    byte red;
    byte green;
    byte blue;

    break_into_parts(background_color, &red, &green, &blue);

    ncchannels_set_bg_rgb8(&channels, (uint32_t)red, (uint32_t)green, (uint32_t)blue);
    ncchannels_set_bg_alpha(&channels, NCALPHA_OPAQUE);
    ncplane_set_base(view->plane, " ", 0, channels);
    ncplane_erase(view->plane);

    view->world_mutex = world_mutex;

#ifdef COOL_EFFECTS
    view->perlin_seed = (float)rand();

    init_cell_matrix(&view->cell_matrix);
    generate_effects(view, size);

    view->angle = 0.0f;
#endif
}

void draw_world_on_view(View *view, World *world) {
    uint32_t rows = 0;
    uint32_t columns = 0;

    ncplane_dim_yx(view->plane, &rows, &columns);

    int32_t half_height = (int32_t)rows / 2;
    int32_t half_width = (int32_t)columns / 2;

    int32_t origin_row = view->position.y - half_height;
    int32_t origin_column = view->position.x - half_width;

#ifdef COOL_EFFECTS
    view->angle += VIEW_EFFECTS_ANGLE_STEP;
    float sin_angle = sinf(view->angle);
    float cos_angle = cosf(view->angle);
#endif

    VectorU world_size = world->cell_matrix.size;

    pthread_mutex_lock(view->world_mutex);
    for (int32_t row = origin_row; row < (int32_t)view->position.y + half_height; row++) {
        for (int32_t column = origin_column; column < (int32_t)view->position.x + half_width; column++) {
            if (row >= 0 && (uint32_t)row < world_size.y && column >= 0 && (uint32_t)column < world_size.x) {
                Cell *cell = get_cell_ref_xy(&world->cell_matrix, row, column);

                cell->animation_index += (1.0f / (float)RENDER_FREQUENCY) * ANIMATION_SPEED;

                update_cell_character(cell);

                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &cell->cell);
            } else {
#ifdef COOL_EFFECTS
                float normalized_row = (float)(row - origin_row - half_height);
                float normalized_column = (float)(column - origin_column - half_width);

                float rotated_row = normalized_row * cos_angle - normalized_column * sin_angle;
                float rotated_column = normalized_row * sin_angle + normalized_column * cos_angle;

                Cell *outsize_cell =
                    get_cell_ref_xy_wrapped(&view->cell_matrix, (int32_t)rotated_row, (int32_t)rotated_column);

                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &outsize_cell->cell);
#else
                ncplane_putc_yx(view->plane, row - origin_row, column - origin_column, &DEFAULT_CELL.cell);
#endif
            }
        }
    }
    pthread_mutex_unlock(view->world_mutex);
}

#ifdef COOL_EFFECTS
void generate_effects(View *view, VectorU size) {
    allocate_cell_matrix(&view->cell_matrix, size);

    for (int32_t row = 0; row < (int32_t)size.y; row++) {
        for (int32_t column = 0; column < (int32_t)size.x; column++) {
            float noise = perlin_noise((float)column * VIEW_PERLIN_NOISE_SCALE,
                                       (float)row * VIEW_PERLIN_NOISE_SCALE,
                                       VIEW_PERLIN_NOISE_PERSISTENCE,
                                       VIEW_PERLIN_NOISE_OCTAVES,
                                       (int)view->perlin_seed);
            Cell cell;

            init_cell(&cell,
                      VIEW_EFFECTS_CHARACTER,
                      VIEW_EFFECTS_CELL_ANIMATION_LENGTH,
                      fabsf(noise) * VIEW_MAX_OUTSIZE_COLOR,
                      VOID);

            set_cell_xy(&view->cell_matrix, row, column, cell);
        }
    }
}
#endif

void free_view(View *view) {
    ncplane_destroy(view->plane);
    view->plane = nullptr;

#ifdef COOL_EFFECTS
    free_cell_matrix(&view->cell_matrix);
#endif
}
