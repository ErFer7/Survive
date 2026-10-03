#pragma once

#include <pthread.h>

#include "types.h"

static const long NANOSECONDS = 1000000000L;

enum Status { FINISHED = -1, SUSPENDED = 0, RUNNING = 1 };

struct PeriodicThread {
    pthread_t thread;
    long period_ns;
    struct timespec next_time;
    void (*function)(void *);
    void *arg;
    enum Status status;
};

static inline long frequency_hz_to_period_ms(int frequency_hz) { return 1000 / frequency_hz; }

// Period in ms
void init_periodic_thread(PeriodicThread *periodic_thread, long period_ms, void (*function)(void *), void *arg);
void start_periodic_thread(PeriodicThread *periodic_thread);
void join_periodic_thread(PeriodicThread *periodic_thread);
void suspend_periodic_thread(PeriodicThread *periodic_thread);
void stop_periodic_thread(PeriodicThread *periodic_thread);
void *run_periodic_thread(void *periodic_thread);
static inline void free_periodic_thread(PeriodicThread *periodic_thread) {}
