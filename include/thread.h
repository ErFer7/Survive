#pragma once

#include <pthread.h>

#include "platform/chronometer.h"

typedef struct {
    pthread_t thread;
    int running;
    float frequency;  // Hz
} Thread;

void create_thread(Thread *thread, float frequency, void *function, void *args);
