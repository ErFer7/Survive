#include "system/periodic_thread.h"

#include <bits/time.h>

void init_periodic_thread(PeriodicThread *periodic_thread, long period_ms, void (*function)(void *), void *arg) {
    periodic_thread->period_ns = period_ms * 1000000UL;

    clock_gettime(CLOCK_MONOTONIC, &periodic_thread->next_time);

    periodic_thread->function = function;
    periodic_thread->arg = arg;
    periodic_thread->status = SUSPENDED;

    pthread_create(&periodic_thread->thread, nullptr, &run_periodic_thread, periodic_thread);
}

void start_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = RUNNING; }

void join_periodic_thread(PeriodicThread *periodic_thread) { pthread_join(periodic_thread->thread, nullptr); }

void suspend_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = SUSPENDED; }

void stop_periodic_thread(PeriodicThread *periodic_thread) { periodic_thread->status = FINISHED; }

void *run_periodic_thread(void *periodic_thread) {
    PeriodicThread *thread = (PeriodicThread *)periodic_thread;

    while (thread->status != FINISHED) {
        if (thread->status == RUNNING) {
            thread->function(thread->arg);
        }

        thread->next_time.tv_sec += thread->period_ns / NANOSECONDS;
        thread->next_time.tv_nsec += thread->period_ns % NANOSECONDS;

        if (thread->next_time.tv_nsec >= NANOSECONDS) {
            thread->next_time.tv_sec++;
            thread->next_time.tv_nsec -= NANOSECONDS;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &thread->next_time, NULL);
    }

    return nullptr;
}
