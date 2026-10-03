#include "scenes/scene.h"

#include "utils/vector.h"

void init_scene(Scene *scene,
                void (*enter)(void *),
                void (*update)(void *),
                void (*draw)(void *),
                void (*exit)(void *),
                Vector parent_size,
                struct ncplane *parent_plane) {
    scene->enter = enter;
    scene->update = update;
    scene->draw = draw;
    scene->exit = exit;

    const struct ncplane_options plane_options =
        {-9999, -9999, parent_size.y, parent_size.x, nullptr, nullptr, nullptr, NCPLANE_OPTION_FIXED, 0, 0};

    memcpy(&scene->plane_options, &plane_options, sizeof(ncplane_options));

    scene->plane = ncplane_create(parent_plane, &scene->plane_options);
}

void free_scene(Scene *scene) { ncplane_destroy(scene->plane); }
