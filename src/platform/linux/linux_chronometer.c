#include "../../../include/platform/linux/linux_chronometer.h"

#include <time.h>

void start_chronometer(Chronometer *chronometer) { clock_gettime(CLOCK_REALTIME, &chronometer->initial_time); }

/*  Retorna a diferença de tempo em microsegundos
 */
double stop_chronometer(Chronometer *chronometer) {
    struct timespec final_time;
    clock_gettime(CLOCK_REALTIME, &final_time);

    return (double)(final_time.tv_nsec - chronometer->initial_time.tv_nsec) / 1000.0;
}
