// Sobreviva - Jogo de Testes - V 2.4 - ErFer7
// Use "gcc Source.c Main.c -o Survive" para compilar.

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

#include "Header.h"

#include <stdio.h>
#include <Windows.h>

int main() {

    GameInit(128);
    BuildMainMenu(MAINMENU);

    while (gameState == MAINMENU) {

        if (GetKeyState(VK_RETURN) & 0x8000) {

            gameState = INGAME;
        }

        Tick(0.0);
    }
    
    GenerateWorld(consoleWidth, consoleHeigth);
    Clear(consoleWidth, consoleHeigth);

    while (gameState == INGAME) {

        LARGE_INTEGER frequency;
        LARGE_INTEGER t1, t2;
        double elapsedTime;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&t1);

        if (GetKeyState(VK_ESCAPE) & 0x8000) {

            gameState = EXIT;
        }

        PlayerControl();

        UpdatePhysics();
        Render();

        QueryPerformanceCounter(&t2);

        // Tempo em milisegundos
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

        float frameTime = Tick(elapsedTime);

        char out[10];
        sprintf(out, "%.3f fps", 1000.0f / frameTime);

        PrintStringOnPosition(out, 7, 0, consoleHeigth - 1);
    }

    FreeObjectArray(&objectArray);

    return 0;
}