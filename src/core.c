#include "../include/core.h"

#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

unsigned int score;
enum State state;
enum Event event;
pthread_mutex_t eventMutex;

void InitCore()
{
    srand((unsigned)time(NULL));

    score = 0;
    state = MAIN_MENU;
    event = IDLE;

    pthread_mutex_init(&eventMutex, NULL);
}

void FreeCore()
{
    pthread_mutex_destroy(&eventMutex);
}

void SetGameEvent(enum Event newEvent, int force)
{
    if (event == IDLE || force)
    {
        event = newEvent;
    }
}

enum Event GetGameEvent()
{
    return event;
}

void LockEvent()
{
    pthread_mutex_lock(&eventMutex);
}

void UnlockEvent()
{
    pthread_mutex_unlock(&eventMutex);
}
