#include "../include/utilities.h"

#include <Windows.h>

void InitTimeContext(TimeContext *timeCtxPtr, float tick)
{
    timeCtxPtr->tick = tick;
    timeCtxPtr->renderingElapsedTime = 0.0;
    timeCtxPtr->behaviourElapsedTime = 0.0;
    timeCtxPtr->physicsElapsedTime = 0.0;
}

void StartChronometer(LARGE_INTEGER *frequency, LARGE_INTEGER *initialTime)
{
    QueryPerformanceFrequency(frequency);
    QueryPerformanceCounter(initialTime);
}

double StopChronometer(LARGE_INTEGER frequency, LARGE_INTEGER initialTime, LARGE_INTEGER *finalTime)
{
    QueryPerformanceCounter(finalTime);
    return (finalTime->QuadPart - initialTime.QuadPart) * 1000.0 / frequency.QuadPart;
}

/*  Controla o tempo de espera entre frames e o retorna.
*/
float Tick(float tick, double elapsedTime)
{
    // Tempo necessário de espera em milisegundos
    int correctionTime = (int)(1000.0 / (double)tick - elapsedTime);

    if (correctionTime > 0) // Caso seja necessário esperar
    {
        Sleep(correctionTime); // Espera
        return 1000.0f / tick;
    }
    else
    {
        return (float)elapsedTime;
    }
}

/*  Gera um número aleatório no intervalo especificado. O número é um float porém é gerado como
    inteiro.
*/
float Randomf(int min, int max)
{
    return (float)((rand() % (max - min + 1)) + min);
}
