#include "../include/statemachine.h"

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/entity.h"
#include "../include/world.h"

void InitEventStateMachine(float tick_, int consoleWidth_, int consoleHeight_)
{
    InitCore();
    SetTick(tick_);
    InitConsoleRenderer(consoleWidth_, consoleHeight_);
    InitInterface();
}

void FreeEventStateMachine()
{
    FreeCore();
    FreeConsoleRenderer();
}

void RunEventStateMachine()
{
    while (state != EXIT)
    {
        UpdateInterfaces();

        LockEvent();
        switch (GetGameEvent())
        {
        case UI_START:

            state = START_MENU;
            break;
        case UI_START_SMALL:

            state = GAMEPLAY;
            PlayEventBehaviourWrapper(128, 128, 0, 0);
            break;
        case UI_START_REGULAR:

            state = GAMEPLAY;
            PlayEventBehaviourWrapper(512, 512, 0, 0);
            break;
        case UI_START_LARGE:

            state = GAMEPLAY;
            PlayEventBehaviourWrapper(2048, 2048, 0, 0);
            break;
        case UI_START_MEGA:

            state = GAMEPLAY;
            PlayEventBehaviourWrapper(8192, 8192, 0, 0);
            break;
        case UI_START_CLASSIC:

            state = GAMEPLAY;
            PlayEventBehaviourWrapper(120, 29, 1, 1);
            break;
        case UI_INFO:

            state = INFO_MENU;
            break;
        case UI_QUIT:

            state = EXIT;
            StopBehaviourThread();
            StopPhysicsThread();
            StopRenderingThread();
            FreeEntityMatrix();
            FreeEntitySemaphores();
            break;
        case UI_PAUSE:

            state = PAUSE;
            StopBehaviourThread();
            StopPhysicsThread();
            StopRenderingThread();
            break;
        case UI_RESUME:

            state = GAMEPLAY;
            InitEntitySemaphores();
            StartBehaviourThread();
            StartPhysicsThread();
            StartRenderingThread(fixedScreen);
            break;
        case UI_RESTART:

            state = GAMEPLAY;
            FreeEntityMatrix();
            GenerateWorld(worldWidth, worldHeight, empty);
            InitEntitySemaphores();
            StartBehaviourThread();
            StartPhysicsThread();
            StartRenderingThread(fixedScreen);
            break;
        case UI_RETURN:

            state = MAIN_MENU;
            FreeEntityMatrix();
            break;
        case GM_GAMEOVER:

            state = GAMEOVER;
            StopBehaviourThread();
            StopPhysicsThread();
            StopRenderingThread();
            FreeEntityMatrix();
            FreeEntitySemaphores();
            break;
        default:
            break;
        }

        SetGameEvent(IDLE, 1);
        UnlockEvent();
    }
}

void PlayEventBehaviourWrapper(int worldWidth_, int worldHeight_, int empty_, int fixedScreen_)
{
    GenerateWorld(worldWidth_, worldHeight_, empty_);
    InitEntitySemaphores();
    StartBehaviourThread();
    StartPhysicsThread();
    StartRenderingThread(fixedScreen_);
}
