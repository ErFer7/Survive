#include "../include/core.h"

unsigned int score = 0;
enum State state = MAIN_MENU;
enum Event events[MAX_EVENTS];

void InitCore()
{
    // Inicializa os eventos
    for (int i = 0; i < MAX_EVENTS; i++)
        events[i] = IDLE;
}
