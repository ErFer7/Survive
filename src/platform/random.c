#include "../../include/platform/random.h"

/*  Gera um número aleatório no intervalo especificado. O número é um float porém é gerado como
    inteiro.
*/
float randomf(int min, int max) { return (float)((rand() % (max - min + 1)) + min); }
