#include "Header.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

void GameInit () {

    GameConsoleInit(&consoleWidth, &consoleHeigth, &consoleHandle);
    ObjectArrayInit(&objectArray, 0);

    srand((unsigned)time(NULL));
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

void BuildMainMenu (enum GameState state) {

    BuildBorders(consoleWidth, consoleHeigth);
    PrintStringOnPosition("S U R V I V E", 12, consoleWidth / 2 - 8, consoleHeigth / 2 - 3);
    PrintStringOnPosition("PLAY [Enter]", 7, consoleWidth / 2 - 6, consoleHeigth / 2 - 1);
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

            if (objectArray.array[i].oldPosition[0] != objectArray.array[i].position[0] || objectArray.array[i].oldPosition[1] != objectArray.array[i].position[1]) {
                
                PrintCharOnPosition(255, 0, objectArray.array[i].oldPosition[0], objectArray.array[i].oldPosition[1]);
            }

            PrintCharOnPosition(objectArray.array[i].c, objectArray.array[i].color, objectArray.array[i].position[0], objectArray.array[i].position[1]);

            objectArray.array[i].oldPosition[0] = objectArray.array[i].position[0];
            objectArray.array[i].oldPosition[1] = objectArray.array[i].position[1];

            objectArray.array[i].updateRender = 0;
        }
    }
}

void GenerateWorld (int width, int heigth) {

    Object player = {254, 15, {width / 2, heigth / 2}, {width / 2, heigth / 2}, PLAYER, 1};
    InsertObjectOnArray(&objectArray, player);

    for (int i = 0; i < width; i++) {

        Object topWall = {219, 7, {i, 0}, {i, 0}, WALL, 1};
        Object bottonWall = {219, 7, {i, heigth - 2}, {i, heigth - 2}, WALL, 1};

        InsertObjectOnArray(&objectArray, topWall);
        InsertObjectOnArray(&objectArray, bottonWall);
    };

    for (int i = 0; i < heigth - 2; i++) {

        Object leftWall = {219, 7, {0, i}, {0, i}, WALL, 1};
        Object rightWall = {219, 7, {width - 1, i}, {width - 1, i}, WALL, 1};

        InsertObjectOnArray(&objectArray, leftWall);
        InsertObjectOnArray(&objectArray, rightWall);
    };  
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

        for (int i = 1; i < objectArray.used; i++) {

            if (movingUp || movingDown || movingRight || movingLeft) {

                if (movingUp && objectArray.array[i].position[1] == objectArray.array[0].position[1] - 1 && objectArray.array[i].position[0] == objectArray.array[0].position[0]) {

                    movingUp = 0;
                }

                if (movingDown && objectArray.array[i].position[1] == objectArray.array[0].position[1] + 1 && objectArray.array[i].position[0] == objectArray.array[0].position[0]) {

                    movingDown = 0;
                }

                if (movingRight && objectArray.array[i].position[0] == objectArray.array[0].position[0] + 1 && objectArray.array[i].position[1] == objectArray.array[0].position[1]) {

                    movingRight = 0;
                }

                if (movingLeft && objectArray.array[i].position[0] == objectArray.array[0].position[0] - 1 && objectArray.array[i].position[1] == objectArray.array[0].position[1]) {

                    movingLeft = 0;
                }
            }
            else {

                break;
            }
        }
    }

    if (movingUp) {

        objectArray.array[0].position[1]--;
        objectArray.array[0].updateRender = 1;
    }

    if (movingDown) {

        objectArray.array[0].position[1]++;
        objectArray.array[0].updateRender = 1;
    }

    if (movingRight) {

        objectArray.array[0].position[0]++;
        objectArray.array[0].updateRender = 1;
    }

    if (movingLeft) {

        objectArray.array[0].position[0]--;
        objectArray.array[0].updateRender = 1;
    }
}