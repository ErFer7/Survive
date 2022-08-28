#include "../include/core.h"

#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*  Inicializa o contexto de eventos e estados
*/
void InitEventStateContext(EventStateContext *eventStateCtx)
{
    srand((unsigned)time(NULL));

    eventStateCtx->event = IDLE;
    eventStateCtx->state = MAIN_MENU;
    pthread_mutex_init(&eventStateCtx->eventMutex, NULL);
}

/*  Libera o contexto de eventos e estados
*/
void FreeEventStateContext(EventStateContext *eventStateCtx)
{
    pthread_mutex_destroy(&eventStateCtx->eventMutex);
}
