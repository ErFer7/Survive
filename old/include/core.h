#pragma once

#include <pthread.h>

// UI: User interface
// IN: Internal
// GP: Gameplay
enum Event {
    UI_START,
    UI_GENERATE,
    UI_INFO,
    UI_QUIT,
    UI_PAUSE,
    UI_RESUME,
    UI_RESTART,
    UI_RETURN,
    IN_GENERATED,
    IN_RESTARTED,
    IN_PAUSED,
    IN_RESUMED,
    GP_GAMEOVER,
    IN_FINISHED,
    IN_FREED
};

enum State {
    MAIN_MENU,
    INFO_MENU,
    START_MENU,
    GENERATING,
    GAMEPLAY,
    PAUSING,
    PAUSE,
    RESUMING,
    RESTARTING,
    RETURNING,
    GAMEOVER,
    FINISHING,
    EXITING,
    EXIT
};

typedef struct {
    enum State state;
    pthread_mutex_t event_mutex;
} EventState;

void init_event_state(EventState *event_state);
void free_event_state(EventState *event_state);

void update_state(EventState *event_state, enum Event event);
