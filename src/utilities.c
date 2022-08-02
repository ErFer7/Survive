#include "../include/utilities.h"

#include <Windows.h>

float tick;

void SetTick(float t)
{
    tick = t;
}

float Tick(double elapsedTime)
{
    /* Controla o tempo de espera entre frames e o retorna.
     */

    // Tempo necessário de espera em milisegundos
    int correctionTime = (int)(1000.0 / (double)tick - elapsedTime);

    if (correctionTime > 0) // Caso seja necessário esperar
    {
        Sleep(correctionTime); // Espera

        return 1000.0f / (double)tick;
    }
    else
    {
        return (float)elapsedTime;
    }
}

float Randomf(int min, int max)
{
    /* Gera um número aleatório no intervalo especificado. O número é um float porém é gerado como
       inteiro.
    */

    return (float)((rand() % (max - min + 1)) + min);
}
