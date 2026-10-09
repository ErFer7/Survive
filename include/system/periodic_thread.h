#pragma once

#include <pthread.h>

#include "types.h"

static const int64_t NANOSECONDS = 1000000000L;

enum Status { FINISHED = -1, SUSPENDED = 0, RUNNING = 1 };

struct PeriodicThread {
    pthread_t thread;
    int64_t period_ns;
    void (*function)(void *);
    void *arg;
    Text *frequency_info;
    struct timespec last_time;
    enum Status status;
};

static inline int64_t frequency_hz_to_period_ms(int64_t frequency_hz) { return 1000 / frequency_hz; }

// Period in ms
void init_periodic_thread(PeriodicThread *periodic_thread, int64_t period_ms, void (*function)(void *), void *arg);

static inline void start_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = RUNNING; }

static inline void join_periodic_thread(PeriodicThread *periodic_thread) {
    pthread_join(periodic_thread->thread, nullptr);
}

static inline void suspend_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = SUSPENDED; }

static inline void stop_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = FINISHED; }

void *run_periodic_thread(void *periodic_thread);

static inline void free_periodic_thread(PeriodicThread *) {}
