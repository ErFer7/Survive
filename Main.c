// Sobreviva - Jogo de Testes - V 2.9.2 - ErFer7

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

int main()
{
    srand((unsigned)time(NULL));
    InitCore();
    SetTick(120);
    InitConsoleRenderer(120, 30);
    InitInterface();

    while (state != EXIT)
    {
        LARGE_INTEGER frequency;
        LARGE_INTEGER t1, t2;
        double elapsedTime;

        // Marca o tempo
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&t1);

        // Atualiza sistemas e renderiza
        if (state == GAMEPLAY)
        {
            UpdateObjectBehaviour();
            UpdatePhysics();
            RenderEntities(tick);
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

                    GenerateWorld(consoleWidth, consoleHeight);
                    state = GAMEPLAY;
                    break;
                case UI_INFO:

                    state = INFO_MENU;
                    break;
                case UI_QUIT:

                    FreeEntityMatrix();
                    state = EXIT;
                    break;
                case UI_PAUSE:

                    state = PAUSE;
                    break;
                case UI_RESUME:

                    state = GAMEPLAY;
                    break;
                case UI_RESTART:

                    FreeEntityMatrix();
                    GenerateWorld(consoleWidth, consoleHeight);
                    state = GAMEPLAY;
                    break;
                case UI_RETURN:

                    state = MAIN_MENU;
                    break;
                case GM_GAMEOVER:

                    FreeEntityMatrix();
                    state = GAMEOVER;
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

        // Marca o tempo
        QueryPerformanceCounter(&t2);

        // Tempo em milisegundos
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

        float frameTime = Tick(elapsedTime); // Controla o fps

        // Define o fps na interface
        sprintf(gameplay.texts[0].content, "%.3f fps   ", 1000.0f / frameTime);
        gameplay.texts[0].update = 1;
    }

    // Libera o console
    FreeConsoleRenderer();

    return 0;
}
