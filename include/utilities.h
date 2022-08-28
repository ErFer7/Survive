#pragma once

#include <Windows.h>

typedef struct
{
    LARGE_INTEGER renderingFrequency;
    LARGE_INTEGER renderingInitialTime;
    double renderingElapsedTime;
    LARGE_INTEGER updateFrequency;
    LARGE_INTEGER updateInitialTime;
    double updateElapsedTime;
} TimeContext;

void InitTimeContext(TimeContext *timeCtxPtr);
void StartStopWatch(LARGE_INTEGER *frequency, LARGE_INTEGER *initialTime);
double StopStopWatch(LARGE_INTEGER frequency, LARGE_INTEGER initialTime);
float Randomf(int min, int max);
