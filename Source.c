#include "Header.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

void GameInit (int t) {

    GameConsoleInit(&consoleWidth, &consoleHeigth, &consoleHandle);
    ObjectArrayInit(&objectArray, 0);

    srand((unsigned)time(NULL));

    tick = t;
    updateMenu = 1;
}

void GameConsoleInit (int *width, int *heigth, HANDLE *handle) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    *handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(*handle, &csbi);

    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *heigth = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void BuildBorders (int width, int heigth) {

    for (int i = 0; i < width; i++) {

        PrintCharOnPosition(219, 7, i, 0);
        PrintCharOnPosition(219, 7, i, heigth - 2);
    };

    for (int i = 0; i < heigth - 1; i++) {

        PrintCharOnPosition(219, 7, 0, i);
        PrintCharOnPosition(219, 7, width - 1, i);
    };
}

void PrintStringOnPosition (char* s, int color, int x, int y) {
    
	COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleTextAttribute(consoleHandle, color);
	SetConsoleCursorPosition(consoleHandle, coord);
    printf("%s", s);
}

void PrintCharOnPosition (char c, int color, int x, int y) {
    
	COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleTextAttribute(consoleHandle, color);
	SetConsoleCursorPosition(consoleHandle, coord);
    putchar(c);
}

void BuildMenu (enum MenuState state) {

    Clear(consoleWidth, consoleHeigth);
    BuildBorders(consoleWidth, consoleHeigth);

    if (state == MAIN) {

        int const titleHorizontalAlignment = 37;

        PrintStringOnPosition("  ______   __    __  _______   __     __  ______  __     __  ________ ",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 10);
        PrintStringOnPosition(" /      \\ |  \\  |  \\|       \\ |  \\   |  \\|      \\|  \\   |  \\|        \\",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 9);
        PrintStringOnPosition("|  $$$$$$\\| $$  | $$| $$$$$$$\\| $$   | $$ \\$$$$$$| $$   | $$| $$$$$$$$",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 8);
        PrintStringOnPosition("| $$___\\$$| $$  | $$| $$__| $$| $$   | $$  | $$  | $$   | $$| $$__    ",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 7);
        PrintStringOnPosition(" \\$$    \\ | $$  | $$| $$    $$ \\$$\\ /  $$  | $$   \\$$\\ /  $$| $$  \\   ",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 6);
        PrintStringOnPosition(" _\\$$$$$$\\| $$  | $$| $$$$$$$\\  \\$$\\  $$   | $$    \\$$\\  $$ | $$$$$   ",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 5);
        PrintStringOnPosition("|  \\__| $$| $$__/ $$| $$  | $$   \\$$ $$   _| $$_    \\$$ $$  | $$_____ ",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 4);
        PrintStringOnPosition(" \\$$    $$ \\$$    $$| $$  | $$    \\$$$   |   $$ \\    \\$$$   | $$     \\",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 3);
        PrintStringOnPosition("  \\$$$$$$   \\$$$$$$  \\$$   \\$$     \\$     \\$$$$$$     \\$     \\$$$$$$$$",
                            12,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 2);

        PrintStringOnPosition("V 2.6", 10, 1, 1);

        PrintStringOnPosition("______________", 4, consoleWidth / 2 - 7, consoleHeigth / 2);
        PrintStringOnPosition("PLAY [Enter]", 7, consoleWidth / 2 - 6, consoleHeigth / 2 + 1);
        PrintStringOnPosition("______________", 4, consoleWidth / 2 - 7, consoleHeigth / 2 + 2);
        PrintStringOnPosition("INFO [I]", 7, consoleWidth / 2 - 4, consoleHeigth / 2 + 3);
        PrintStringOnPosition("______________", 4, consoleWidth / 2 - 7, consoleHeigth / 2 + 4);
        PrintStringOnPosition("QUIT [ESC]", 7, consoleWidth / 2 - 5, consoleHeigth / 2 + 5);
    }
    else {

        int const titleHorizontalAlignment = 20;

        PrintStringOnPosition(" ______  __    __  ________   ______  ",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 10);
        PrintStringOnPosition("|      \\|  \\  |  \\|        \\ /      \\ ",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 9);
        PrintStringOnPosition(" \\$$$$$$| $$\\ | $$| $$$$$$$$|  $$$$$$\\",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 8);
        PrintStringOnPosition("  | $$  | $$$\\| $$| $$__    | $$  | $$",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 7);
        PrintStringOnPosition("  | $$  | $$$$\\ $$| $$  \\   | $$  | $$",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 6);
        PrintStringOnPosition("  | $$  | $$\\$$ $$| $$$$$   | $$  | $$",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 5);
        PrintStringOnPosition(" _| $$_ | $$ \\$$$$| $$      | $$__/ $$",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 4);
        PrintStringOnPosition("|   $$ \\| $$  \\$$$| $$       \\$$    $$",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 3);
        PrintStringOnPosition(" \\$$$$$$ \\$$   \\$$ \\$$        \\$$$$$$ ",
                            10,
                            consoleWidth / 2 - titleHorizontalAlignment,
                            consoleHeigth / 2 - 2);

        PrintStringOnPosition("Version 2.6, built in 13/12/2020", 7, consoleWidth / 2 - 15, consoleHeigth / 2 + 1);
        PrintStringOnPosition("Developed by Eric Fernandes Evaristo (ErFer7)", 7, consoleWidth / 2 - 21, consoleHeigth / 2 + 2);
        PrintStringOnPosition("Writen in C", 7, consoleWidth / 2 - 6, consoleHeigth / 2 + 3);
        PrintStringOnPosition("Menu [ESC]", 7, consoleWidth / 2 - 5, consoleHeigth / 2 + 5);
    }

    updateMenu = 0;
}

void Clear (int width, int heigth) {

    for (int i = 0; i < width; i++) {

        for (int j = 0; j < heigth; j++) {

            PrintCharOnPosition(255, 7, i, j);
        }
    }
}

void Render () {

    for (int i = 0; i < objectArray.used; i++) {

        if (objectArray.array[i].updateRender) {

            PrintCharOnPosition(objectArray.array[i].c,
                                objectArray.array[i].color,
                                objectArray.array[i].position[0],
                                objectArray.array[i].position[1]);

            if (objectArray.array[i].oldPosition[0] != objectArray.array[i].position[0] ||
                objectArray.array[i].oldPosition[1] != objectArray.array[i].position[1]) {

                PrintCharOnPosition(255,
                                    0,
                                    objectArray.array[i].oldPosition[0],
                                    objectArray.array[i].oldPosition[1]);
            }

            objectArray.array[i].oldPosition[0] = objectArray.array[i].position[0];
            objectArray.array[i].oldPosition[1] = objectArray.array[i].position[1];

            objectArray.array[i].updateRender = 0;
        }
    }

    PrintCharOnPosition(255, 0, consoleWidth - 2, consoleHeigth - 1);
}

void GenerateWorld (int width, int heigth) {

    Object player = {254,
                     15,
                     {width / 2, heigth / 2},
                     {width / 2, heigth / 2},
                     {(float)(width / 2), (float)(heigth / 2)},
                     0.5f,
                     PLAYER,
                     1};

    Object coin = {254,
                   14,
                   {RandomInt(1, consoleWidth - 2), RandomInt(1, consoleHeigth - 3)},
                   {RandomInt(1, consoleWidth - 2), RandomInt(1, consoleHeigth - 3)},
                   {(float)(RandomInt(1, consoleWidth - 2)), (float)(RandomInt(1, consoleHeigth - 3))},
                   0.0f,
                   COIN,
                   1};

    InsertObjectOnArray(&objectArray, player);
    InsertObjectOnArray(&objectArray, coin);

    for (int i = 0; i < 100; i++) {

        Object testMonster = {254,
                            12,
                            {RandomInt(1, consoleWidth - 2), RandomInt(1, consoleHeigth - 3)},
                            {RandomInt(1, consoleWidth - 2), RandomInt(1, consoleHeigth - 3)},
                            {(float)(RandomInt(1, consoleWidth - 2)), (float)(RandomInt(1, consoleHeigth - 3))},
                            0.1f,
                            ENEMY,
                            1};

        InsertObjectOnArray(&objectArray, testMonster);
    }

    for (int i = 0; i < width; i++) {

        Object topWall = {219, 7, {i, 0}, {i, 0}, {(float)i, 0.0f}, 0.0f, WALL, 1};

        Object bottonWall = {219,
                             7,
                             {i, heigth - 2},
                             {i, heigth - 2},
                             {(float)i, (float)(heigth - 2)},
                             0.0f,
                             WALL,
                             1};

        InsertObjectOnArray(&objectArray, topWall);
        InsertObjectOnArray(&objectArray, bottonWall);
    }

    for (int i = 0; i < heigth - 2; i++) {

        Object leftWall = {219, 7, {0, i}, {0, i}, {0.0f, (float)i}, 0.0f, WALL, 1};
        Object rightWall = {219,
                            7,
                            {width - 1, i},
                            {width - 1, i},
                            {(float)(width - 1), (float)i},
                            0.0f,
                            WALL,
                            1};

        InsertObjectOnArray(&objectArray, leftWall);
        InsertObjectOnArray(&objectArray, rightWall);
    }
}

void ObjectArrayInit (ObjectArray *objectArray, size_t initialSize) {

    objectArray -> array = (Object*)malloc(initialSize * sizeof(Object));
    objectArray -> used = 0;
    objectArray -> size = initialSize;
}

void InsertObjectOnArray (ObjectArray *objectArray, Object object) {

    if (objectArray -> used == objectArray -> size) {

        objectArray -> size++;
        objectArray -> array = (Object*)realloc(objectArray -> array, objectArray -> size * sizeof(Object));
    }

    objectArray -> array[objectArray -> used++] = object;
}

void FreeObjectArray (ObjectArray *objectArray) {


    free(objectArray -> array);
    objectArray -> array = NULL;
    objectArray -> used = 0;
    objectArray -> size = 0;
}

void PlayerControl () {

    int movingUp = 0, movingDown = 0, movingRight = 0, movingLeft = 0, moved = 0;

    if (GetKeyState(VK_UP) & 0x8000) {

        movingUp = 1;
        moved = 1;
    }
    if (GetKeyState(VK_DOWN) & 0x8000) {

        movingDown = 1;
        moved = 1;
    }
    if (GetKeyState(VK_RIGHT) & 0x8000) {

        movingRight = 1;
        moved = 1;
    }
    if (GetKeyState(VK_LEFT) & 0x8000) {

        movingLeft = 1;
        moved = 1;
    }

    if (moved) {

        if (movingUp && objectArray.array[0].position[1] == 1) {

            movingUp = 0;
        }

        if (movingDown && objectArray.array[0].position[1] == consoleHeigth - 3) {

            movingDown = 0;
        }

        if (movingRight && objectArray.array[0].position[0] == consoleWidth - 2) {

            movingRight = 0;
        }

        if (movingLeft && objectArray.array[0].position[0] == 1) {

            movingLeft = 0;
        }
    }

    if (movingUp) {

        objectArray.array[0].interPosition[1] -= objectArray.array[0].speed;
        objectArray.array[0].updateRender = 1;
    }

    if (movingDown) {

        objectArray.array[0].interPosition[1] += objectArray.array[0].speed;
        objectArray.array[0].updateRender = 1;
    }

    if (movingRight) {

        objectArray.array[0].interPosition[0] += objectArray.array[0].speed;
        objectArray.array[0].updateRender = 1;
    }

    if (movingLeft) {

        objectArray.array[0].interPosition[0] -= objectArray.array[0].speed;
        objectArray.array[0].updateRender = 1;
    }
}

void EnemyBehaviour(int monsterIndex) {

    int movingUp = 0, movingDown = 0, movingRight = 0, movingLeft = 0, moved = 0;

    if (objectArray.array[0].position[1] < objectArray.array[monsterIndex].position[1]) {

        movingUp = 1;
        moved = 1;
    }
    if (objectArray.array[0].position[1] > objectArray.array[monsterIndex].position[1]) {

        movingDown = 1;
        moved = 1;
    }
    if (objectArray.array[0].position[0] > objectArray.array[monsterIndex].position[0]) {

        movingRight = 1;
        moved = 1;
    }
    if (objectArray.array[0].position[0] < objectArray.array[monsterIndex].position[0]) {

        movingLeft = 1;
        moved = 1;
    }

    if (moved) {

        for (int i = 0; i < objectArray.used; i++) {

            if ((movingUp || movingDown || movingRight || movingLeft) && i != monsterIndex) {

                if (movingUp &&
                    objectArray.array[i].position[1] == objectArray.array[monsterIndex].position[1] - 1 &&
                    objectArray.array[i].position[0] == objectArray.array[monsterIndex].position[0]) {

                    movingUp = 0;
                }

                if (movingDown &&
                    objectArray.array[i].position[1] == objectArray.array[monsterIndex].position[1] + 1 &&
                    objectArray.array[i].position[0] == objectArray.array[monsterIndex].position[0]) {

                    movingDown = 0;
                }

                if (movingRight &&
                    objectArray.array[i].position[0] == objectArray.array[monsterIndex].position[0] + 1 &&
                    objectArray.array[i].position[1] == objectArray.array[monsterIndex].position[1]) {

                    movingRight = 0;
                }

                if (movingLeft &&
                    objectArray.array[i].position[0] == objectArray.array[monsterIndex].position[0] - 1 &&
                    objectArray.array[i].position[1] == objectArray.array[monsterIndex].position[1]) {

                    movingLeft = 0;
                }
            }
            else {

                break;
            }
        }
    }

    if (movingUp) {

        objectArray.array[monsterIndex].interPosition[1] -= objectArray.array[monsterIndex].speed;
        objectArray.array[monsterIndex].updateRender = 1;
    }

    if (movingDown) {

        objectArray.array[monsterIndex].interPosition[1] += objectArray.array[monsterIndex].speed;
        objectArray.array[monsterIndex].updateRender = 1;
    }

    if (movingRight) {

        objectArray.array[monsterIndex].interPosition[0] += objectArray.array[monsterIndex].speed;
        objectArray.array[monsterIndex].updateRender = 1;
    }

    if (movingLeft) {

        objectArray.array[monsterIndex].interPosition[0] -= objectArray.array[monsterIndex].speed;
        objectArray.array[monsterIndex].updateRender = 1;
    }
}

float Tick(double elapsedTime) {

    int correctionTime = (int)(1000.0 / (double)tick - elapsedTime);

    if (correctionTime > 0) {

        Sleep(correctionTime);

        return (float)tick;
    }
    else {

        return (float)elapsedTime;
    }
}

void UpdatePhysics() {

    for (int i = 0; i < objectArray.used; i++) {

        if (objectArray.array[i].type == ENEMY) {

            EnemyBehaviour(i);
        }

        objectArray.array[i].position[0] = (int)objectArray.array[i].interPosition[0];
        objectArray.array[i].position[1] = (int)objectArray.array[i].interPosition[1];
    }
}

int RandomInt(int min, int max) {

    return rand() % (max + 1 - min) + min;
}