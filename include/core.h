#pragma once

#define MAX_EVENTS 2

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
extern enum Event events[MAX_EVENTS];
extern unsigned int score;

void InitCore();
