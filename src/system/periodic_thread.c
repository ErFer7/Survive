#include "system/periodic_thread.h"

#include <bits/time.h>
#include <stdio.h>

#include "interface/text.h"

void init_periodic_thread(PeriodicThread *periodic_thread, long period_ms, void (*function)(void *), void *arg) {
    periodic_thread->period_ns = period_ms * 1000000UL;
    periodic_thread->function = function;
    periodic_thread->arg = arg;
    periodic_thread->status = SUSPENDED;

    clock_gettime(CLOCK_MONOTONIC, &periodic_thread->last_time);

    pthread_create(&periodic_thread->thread, nullptr, &run_periodic_thread, periodic_thread);
}

void *run_periodic_thread(void *periodic_thread) {
    PeriodicThread *thread = (PeriodicThread *)periodic_thread;

    char frequency_info_str[11];

    struct timespec start;
    struct timespec end;
    struct timespec wait;

    while (thread->status != FINISHED) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        if (thread->status == RUNNING) {
            thread->function(thread->arg);

            int64_t diff_last_time_ns = (int64_t)(start.tv_sec - thread->last_time.tv_sec) * NANOSECONDS +
                                        (start.tv_nsec - thread->last_time.tv_nsec);

            snprintf(frequency_info_str,
                     sizeof(frequency_info_str),
                     "%010.3f",
                     (float)NANOSECONDS / (float)diff_last_time_ns);
            set_single_line_text_content(thread->frequency_info, frequency_info_str, sizeof(frequency_info_str));

            thread->last_time = start;
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
