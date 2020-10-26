// Sobreviva - Jogo de Testes - V1.2.6 - ErFer7
// Use "gcc Source.c Main.c -o Survive" para compilar.
// V 1.3 - INDEV

#include "Header.h"

#include <stdio.h>
#include <Windows.h>

int main() {

    GameInit();
    BuildMainMenu(MAINMENU);

    while (gameState == MAINMENU) {

        if (GetKeyState(VK_RETURN) & 0x8000) {

            gameState = INGAME;
        }

        Sleep(1);
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

        Render();

        QueryPerformanceCounter(&t2);
        elapsedTime = 1000.0 / ((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);
        char out[25];
        sprintf(out, "%.3f fps", elapsedTime);

        PrintStringOnPosition(out, 7, 0, consoleHeigth - 1);
        Sleep(1);
    }

    FreeObjectArray(&objectArray);

    return 0;
}