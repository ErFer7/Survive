#pragma once

#include <pthread.h>

enum Event
{
    IDLE,
    UI_START,
    UI_START_SMALL,
    UI_START_REGULAR,
    UI_START_LARGE,
    UI_START_MEGA,
    UI_START_CLASSIC,
    UI_INFO,
    UI_QUIT,
    UI_PAUSE,
    UI_RESUME,
    UI_RESTART,
    UI_RETURN,
    GM_GAMEOVER
};

enum State
{
    MAIN_MENU,
    INFO_MENU,
    START_MENU,
    GAMEPLAY,
    PAUSE,
    GAMEOVER,
    EXIT
};

typedef struct
{
    enum Event event;
    enum State state;
    pthread_mutex_t eventMutex;
} EventStateContext;

void InitEventStateContext(EventStateContext *eventStateCtx);
void FreeEventStateContext(EventStateContext *eventStateCtx);
