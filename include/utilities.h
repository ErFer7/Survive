#pragma once

#include <Windows.h>

extern float tick;
extern LARGE_INTEGER renderingFrequency;
extern LARGE_INTEGER renderingInitialTime, renderingFinalTime;
extern double renderingElapsedTime;
extern LARGE_INTEGER behaviourFrequency;
extern LARGE_INTEGER behaviourInitialTime, behaviourFinalTime;
extern double behaviourElapsedTime;
extern LARGE_INTEGER physicsFrequency;
extern LARGE_INTEGER physicsInitialTime, physicsFinalTime;
extern double physicsElapsedTime;

typedef struct
{
    float tick;
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

void SetTick(float t);
void StartChronometer(LARGE_INTEGER *frequency, LARGE_INTEGER *initialTime);
double StopChronometer(LARGE_INTEGER frequency, LARGE_INTEGER initialTime, LARGE_INTEGER *finalTime);
float Tick(double elapsedTime);
float Randomf(int min, int max);
