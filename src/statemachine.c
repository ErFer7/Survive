#include "../include/statemachine.h"

#include <pthread.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"
#include "../include/world.h"

void InitEventStateMachine(EventStateContext *eventStateCtxPtr,
                           ConsoleContext *consoleCtxPtr,
                           float tick_,
                           int consoleWidth,
                           int consoleHeight)
{
    InitEventStateContext(eventStateCtxPtr);
    SetTick(tick_);
    InitConsoleContext(consoleCtxPtr, CreateVector2D(consoleWidth, consoleHeight));
    InitInterface();
}

void FreeEventStateMachine(EventStateContext *eventStateCtxPtr, ConsoleContext *consoleCtxPtr)
{
    FreeEventStateContext(eventStateCtxPtr);
    FreeConsoleContext(consoleCtxPtr);
}

void RunEventStateMachine(EventStateContext *eventStateCtxPtr, ConsoleContext *consoleCtxPtr)
{
    GameplayContext gameplayCtx;
    ThreadsContext threadsCtx;

    while (eventStateCtxPtr->state != EXIT)
    {
        UpdateInterfaces(eventStateCtxPtr);

        pthread_mutex_lock(&eventStateCtxPtr->eventMutex);
        switch (eventStateCtxPtr->event)
        {
        case UI_START:

            eventStateCtxPtr->state = START_MENU;
            break;
        case UI_START_SMALL:

            eventStateCtxPtr->state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(128, 128), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_START_REGULAR:

            eventStateCtxPtr->state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(512, 512), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_START_LARGE:

            eventStateCtxPtr->state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(2048, 2048), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_START_MEGA:

            eventStateCtxPtr->state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(8192, 8192), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_START_CLASSIC:

            eventStateCtxPtr->state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(120, 29), 1, 1);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_INFO:

            eventStateCtxPtr->state = INFO_MENU;
            break;
        case UI_QUIT:

            eventStateCtxPtr->state = EXIT;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            FreeEntitySemaphores(&threadsCtx);
            FreeGameplayContext(&gameplayCtx);
            break;
        case UI_PAUSE:

            eventStateCtxPtr->state = PAUSE;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            break;
        case UI_RESUME:

            eventStateCtxPtr->state = GAMEPLAY;
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_RESTART:

            eventStateCtxPtr->state = GAMEPLAY;
            Vector2D tempSize = gameplayCtx.worldSize;
            int tempFixed = gameplayCtx.fixedScreen;
            int tempEmpty = gameplayCtx.empty;
            FreeGameplayContext(&gameplayCtx);
            InitGameplayContext(&gameplayCtx, tempSize, tempFixed, tempEmpty);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartPhysicsThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx);
            StartRenderingThread(eventStateCtxPtr, &gameplayCtx, &threadsCtx, consoleCtxPtr);
            break;
        case UI_RETURN:

            eventStateCtxPtr->state = MAIN_MENU;
            FreeGameplayContext(&gameplayCtx);
            break;
        case GM_GAMEOVER:

            eventStateCtxPtr->state = GAMEOVER;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            FreeEntitySemaphores(&threadsCtx);
            FreeGameplayContext(&gameplayCtx);
            break;
        default:
            break;
        }

        eventStateCtxPtr->event = IDLE;
        pthread_mutex_unlock(&eventStateCtxPtr->eventMutex);
    }
}
