#pragma once

#include <Windows.h>

extern float tick;
extern LARGE_INTEGER renderingFrequency;
extern LARGE_INTEGER renderingInitialTime, renderingFinalTime;
extern double renderingElapsedTime;
extern LARGE_INTEGER behaviourFrequency;
extern LARGE_INTEGER behaviourInitialTime, behaviourFinalTime;
extern double behaviourElapsedTime;
extern LARGE_INTEGER tickFrequency;
extern LARGE_INTEGER tickInitialTime, tickFinalTime;
extern double tickElapsedTime;

void SetTick(float t);
void StartChronometer(LARGE_INTEGER *frequency, LARGE_INTEGER *initialTime);
double StopChronometer(LARGE_INTEGER frequency, LARGE_INTEGER initialTime, LARGE_INTEGER *finalTime);
float Tick(double elapsedTime);
float Randomf(int min, int max);
