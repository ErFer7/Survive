#include "../include/core.h"

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

void init_event_state(EventState *event_state) {
    srand((unsigned)time(NULL));

    event_state->state = MAIN_MENU;
    pthread_mutex_init(&event_state->event_mutex, NULL);
}

void free_event_state(EventState *event_state) { pthread_mutex_destroy(&event_state->event_mutex); }

void update_state(EventState *event_state, enum Event event) {
    pthread_mutex_lock(&event_state->event_mutex);

    switch (event) {
        case UI_START:
            event_state->state = START_MENU;
            break;
        case UI_GENERATE:
            event_state->state = GENERATING;
            break;
        case UI_INFO:
            event_state->state = INFO_MENU;
            break;
        case UI_QUIT:
            event_state->state = EXITING;
            break;
        case UI_PAUSE:
            event_state->state = PAUSING;
            break;
        case UI_RESUME:
            event_state->state = RESUMING;
            break;
        case UI_RESTART:
            event_state->state = RESTARTING;
            break;
        case UI_RETURN:
            event_state->state = RETURNING;
            break;
        case IN_GENERATED:
        case IN_RESTARTED:
        case IN_RESUMED:
            event_state->state = GAMEPLAY;
            break;
        case IN_PAUSED:
            event_state->state = PAUSE;
            break;
        case GP_GAMEOVER:
            event_state->state = FINISHING;
            break;
        case IN_FINISHED:
            event_state->state = GAMEOVER;
            break;
        case IN_FREED:
            event_state->state = EXIT;
            break;
        default:
            break;
    }

    pthread_mutex_unlock(&event_state->event_mutex);
}
