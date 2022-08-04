// Sobreviva - Jogo de Testes - v2.12-dev - ErFer7

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

#include <stdio.h>

#include <time.h>
#include <Windows.h>

#include "include/core.h"
#include "include/utilities.h"
#include "include/graphics.h"
#include "include/interface.h"
#include "include/entity.h"
#include "include/world.h"

#define TICK 120
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 30
#define WORLD_WIDTH 512
#define WORLD_HEIGHT 512
#define SIMULATION_AREA_WIDTH 128
#define SIMULATION_AREA_HEIGHT 128

int main()
{
    srand((unsigned)time(NULL));
    InitCore();
    SetTick(TICK);
    InitConsoleRenderer(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    InitInterface();
    SetSimulationAreaSize(SIMULATION_AREA_WIDTH, SIMULATION_AREA_HEIGHT);

    while (state != EXIT)
    {
        StartChronometer(&tickFrequency, &tickInitialTime);

        // Atualiza sistemas e renderiza
        if (state == GAMEPLAY)
        {
            UpdateEntityBehaviour();
            UpdatePhysics();
        }

        UpdateInterfaces();

        // Máquina de estados com base em eventos
        for (int i = 0; i < MAX_EVENTS; i++)
        {
            if (events[i] != IDLE)
            {
                switch (events[i])
                {
                case UI_PLAY:

                    state = GAMEPLAY;
                    GenerateWorld(WORLD_WIDTH, WORLD_HEIGHT);
                    StartRenderingThread();
                    break;
                case UI_INFO:

                    state = INFO_MENU;
                    break;
                case UI_QUIT:

                    state = EXIT;
                    FreeEntityMatrix();
                    StopRenderingThread();
                    break;
                case UI_PAUSE:

                    state = PAUSE;
                    StopRenderingThread();
                    break;
                case UI_RESUME:

                    state = GAMEPLAY;
                    StartRenderingThread();
                    break;
                case UI_RESTART:

                    state = GAMEPLAY;
                    FreeEntityMatrix();
                    GenerateWorld(WORLD_WIDTH, WORLD_HEIGHT);
                    StartRenderingThread();
                    break;
                case UI_RETURN:

                    state = MAIN_MENU;
                    break;
                case GM_GAMEOVER:

                    state = GAMEOVER;
                    FreeEntityMatrix();
                    StopRenderingThread();
                    break;
                default:
                    break;
                }

                break;
            }
        }

        /* Segundo passe para limpar os eventos. Este passe é necessário pois o loop acima
           quebra quando encontra um evento.
        */
        for (int i = 0; i < MAX_EVENTS; i++)
            events[i] = IDLE;

        float frameTime = Tick(StopChronometer(tickFrequency, tickInitialTime, &tickFinalTime)); // Controla o fps

        // Define o fps na interface
        sprintf(gameplay.texts[1].content, "%012.3f tps", 1000.0f / frameTime);
        gameplay.texts[1].update = 1;
    }

    // Libera o console
    FreeConsoleRenderer();

    return 0;
}
