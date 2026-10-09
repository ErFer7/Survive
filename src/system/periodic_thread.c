#include "system/periodic_thread.h"

#include <bits/time.h>

void init_periodic_thread(PeriodicThread *periodic_thread, long period_ms, void (*function)(void *), void *arg) {
    periodic_thread->period_ns = period_ms * 1000000UL;
    periodic_thread->function = function;
    periodic_thread->arg = arg;
    periodic_thread->status = SUSPENDED;

    pthread_create(&periodic_thread->thread, nullptr, &run_periodic_thread, periodic_thread);
}

void *run_periodic_thread(void *periodic_thread) {
    PeriodicThread *thread = (PeriodicThread *)periodic_thread;

    struct timespec start;
    struct timespec end;
    struct timespec wait;

    while (thread->status != FINISHED) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        if (thread->status == RUNNING) {
            thread->function(thread->arg);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        int64_t diff_ns = (int64_t)(end.tv_sec - start.tv_sec) * NANOSECONDS + (end.tv_nsec - start.tv_nsec);

        if (diff_ns < thread->period_ns) {
            int64_t remaining_ns = thread->period_ns - diff_ns;

            wait.tv_sec = remaining_ns / NANOSECONDS;
            wait.tv_nsec = remaining_ns % NANOSECONDS;

            clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
        }
    }

    return nullptr;
}
