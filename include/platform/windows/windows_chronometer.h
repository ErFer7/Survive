#pragma once

#include <Windows.h>

typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER initial_time;
} Chronometer;

void start_chronometer(Chronometer *chronometer);
double stop_chronometer(Chronometer *chronometer);
