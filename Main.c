// Sobreviva - ErFer7

/***
 *     $$$$$$\   $$$$$$\  $$$$$$$\  $$$$$$$\  $$$$$$$$\ $$\    $$\ $$$$$$\ $$\    $$\  $$$$$$\
 *    $$  __$$\ $$  __$$\ $$  __$$\ $$  __$$\ $$  _____|$$ |   $$ |\_$$  _|$$ |   $$ |$$  __$$\
 *    $$ /  \__|$$ /  $$ |$$ |  $$ |$$ |  $$ |$$ |      $$ |   $$ |  $$ |  $$ |   $$ |$$ /  $$ |
 *    \$$$$$$\  $$ |  $$ |$$$$$$$\ |$$$$$$$  |$$$$$\    \$$\  $$  |  $$ |  \$$\  $$  |$$$$$$$$ |
 *     \____$$\ $$ |  $$ |$$  __$$\ $$  __$$< $$  __|    \$$\$$  /   $$ |   \$$\$$  / $$  __$$ |
 *    $$\   $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |        \$$$  /    $$ |    \$$$  /  $$ |  $$ |
 *    \$$$$$$  | $$$$$$  |$$$$$$$  |$$ |  $$ |$$$$$$$$\    \$  /   $$$$$$\    \$  /   $$ |  $$ |
 *     \______/  \______/ \_______/ \__|  \__|\________|    \_/    \______|    \_/    \__|  \__|
 */

#include <pthread.h>

#include "include/vector2D.h"
#include "include/core.h"
#include "include/utilities.h"
#include "include/graphics.h"
#include "include/interface.h"
#include "include/entity.h"
#include "include/world.h"

#define TICK 120
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 30

int main()
{
    EventStateContext eventStateCtx;
    TimeContext timeCtx;
    ConsoleContext consoleCtx;
    InterfaceContext interfaceCtx;
    GameplayContext gameplayCtx;
    ThreadsContext threadsCtx;

    InitEventStateContext(&eventStateCtx);
    InitTimeContext(&timeCtx, TICK);
    InitConsoleContext(&consoleCtx, CreateVector2D(CONSOLE_WIDTH, CONSOLE_HEIGHT));
    InitInterfaceContext(&interfaceCtx, consoleCtx.size);
    PreInitGameplayContext(&gameplayCtx);

    while (eventStateCtx.state != EXIT)
    {
        UpdateInterfaces(&eventStateCtx, &interfaceCtx, &consoleCtx);

        pthread_mutex_lock(&eventStateCtx.eventMutex);
        switch (eventStateCtx.event)
        {
        case UI_START:

            eventStateCtx.state = START_MENU;
            break;
        case UI_START_SMALL:

            eventStateCtx.state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(128, 128), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_START_REGULAR:

            eventStateCtx.state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(512, 512), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_START_LARGE:

            eventStateCtx.state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(2048, 2048), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_START_MEGA:

            eventStateCtx.state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(8192, 8192), 0, 0);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_START_CLASSIC:

            eventStateCtx.state = GAMEPLAY;
            InitGameplayContext(&gameplayCtx, CreateVector2D(120, 29), 1, 1);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_INFO:

            eventStateCtx.state = INFO_MENU;
            break;
        case UI_QUIT:

            eventStateCtx.state = EXIT;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            FreeEntitySemaphores(&threadsCtx);
            FreeGameplayContext(&gameplayCtx);
            break;
        case UI_PAUSE:

            eventStateCtx.state = PAUSE;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            break;
        case UI_RESUME:

            eventStateCtx.state = GAMEPLAY;
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_RESTART:

            eventStateCtx.state = GAMEPLAY;
            Vector2D tempSize = gameplayCtx.worldSize;
            int tempFixed = gameplayCtx.fixedScreen;
            int tempEmpty = gameplayCtx.empty;
            FreeGameplayContext(&gameplayCtx);
            InitGameplayContext(&gameplayCtx, tempSize, tempFixed, tempEmpty);
            GenerateWorld(&gameplayCtx);
            InitEntitySemaphores(&threadsCtx);
            StartBehaviourThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartPhysicsThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &interfaceCtx, &timeCtx);
            StartRenderingThread(&eventStateCtx, &gameplayCtx, &threadsCtx, &consoleCtx, &interfaceCtx, &timeCtx);
            break;
        case UI_RETURN:

            eventStateCtx.state = MAIN_MENU;
            FreeGameplayContext(&gameplayCtx);
            break;
        case GM_GAMEOVER:

            eventStateCtx.state = GAMEOVER;
            StopBehaviourThread(&threadsCtx);
            StopPhysicsThread(&threadsCtx);
            StopRenderingThread(&threadsCtx);
            FreeEntitySemaphores(&threadsCtx);
            FreeGameplayContext(&gameplayCtx);
            break;
        default:
            break;
        }

        eventStateCtx.event = IDLE;
        pthread_mutex_unlock(&eventStateCtx.eventMutex);
    }

    FreeEventStateContext(&eventStateCtx);
    FreeConsoleContext(&consoleCtx);
    FreeInterfaceContext(&interfaceCtx);

    return 0;
}
