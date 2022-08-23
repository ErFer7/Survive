#pragma once

#include <Windows.h>

typedef struct
{
    LARGE_INTEGER renderingFrequency;
    LARGE_INTEGER renderingInitialTime;
    LARGE_INTEGER renderingFinalTime;
    double renderingElapsedTime;
    LARGE_INTEGER behaviourFrequency;
    LARGE_INTEGER behaviourInitialTime;
    LARGE_INTEGER behaviourFinalTime;
    double behaviourElapsedTime;
    LARGE_INTEGER physicsFrequency;
    LARGE_INTEGER physicsInitialTime;
    LARGE_INTEGER physicsFinalTime;
    double physicsElapsedTime;
} TimeContext;

void InitTimeContext(TimeContext *timeCtxPtr);
void StartChronometer(LARGE_INTEGER *frequency, LARGE_INTEGER *initialTime);
double StopChronometer(LARGE_INTEGER frequency, LARGE_INTEGER initialTime, LARGE_INTEGER *finalTime);
float Randomf(int min, int max);
