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

extern enum State state;
extern enum Event event;
extern unsigned int score;
extern pthread_mutex_t eventMutex;

void InitCore();
void FreeCore();
void SetGameEvent(enum Event event_, int force);
enum Event GetGameEvent();
void LockEvent();
void UnlockEvent();
