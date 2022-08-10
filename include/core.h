#pragma once

#include <pthread.h>

enum Event
{
    IDLE,
    UI_PLAY,
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
void SetGameEvent(enum Event newEvent, int force);
enum Event GetGameEvent();
void LockEvent();
void UnlockEvent();
