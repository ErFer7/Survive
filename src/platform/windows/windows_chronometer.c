#include "../../../include/platform/windows/windows_chronometer.h"

#include <Windows.h>

void start_chronometer(Chronometer *chronometer) {
    QueryPerformanceFrequency(chronometer->frequency);
    QueryPerformanceCounter(chronometer->initial_time);
}

/* Retorna a diferença de tempo em microsegundos
 */
double stop_chronometer(Chronometer *chronometer) {
    LARGE_INTEGER final_time;
    QueryPerformanceCounter(&final_time);

    return (final_time.QuadPart - chronometer->initial_time.QuadPart) * 1000000.0 / chronometer->frequency.QuadPart;
}
