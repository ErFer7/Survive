#pragma once

#include <time.h>

typedef struct {
    struct timespec initial_time;
} Chronometer;

void start_chronometer(Chronometer *chronometer);
double stop_chronometer(Chronometer *chronometer);
