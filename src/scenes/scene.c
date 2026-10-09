#include "scenes/scene.h"

#include "utils/color.h"
#include "utils/vector.h"

void init_scene(Scene *scene,
                void (*enter)(void *, void *),
                void (*update)(void *),
                void (*draw)(void *),
                void (*exit)(void *, void *),
                VectorU parent_size,
                enum Color background_color,
                struct ncplane *parent_plane) {
    scene->enter = enter;
    scene->update = update;
    scene->draw = draw;
    scene->exit = exit;

    const struct ncplane_options plane_options =
        {-9999, -9999, parent_size.y, parent_size.x, nullptr, nullptr, nullptr, NCPLANE_OPTION_FIXED, 0, 0};

    memcpy(&scene->plane_options, &plane_options, sizeof(ncplane_options));

    scene->plane = ncplane_create(parent_plane, &scene->plane_options);

    uint64_t channels = 0;
    byte red;
    byte green;
    byte blue;

    break_into_parts(background_color, &red, &green, &blue);

    ncchannels_set_bg_rgb8(&channels, (uint32_t)red, (uint32_t)green, (uint32_t)blue);
    ncchannels_set_bg_alpha(&channels, NCALPHA_OPAQUE);
    ncplane_set_base(scene->plane, " ", 0, channels);
    ncplane_erase(scene->plane);
}

void free_scene(Scene *scene) { ncplane_destroy(scene->plane); }
