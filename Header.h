#pragma once

#include <Windows.h>

enum GameState {MAINMENU, INGAME, EXIT};
enum ObjectType {WALL, PLAYER, ENEMY, COIN};

typedef struct {

    char c;
    int color;
    int position[2];
    int oldPosition[2];
    enum ObjectType type;
    int updateRender;
    
} Object;

typedef struct {

    Object *array;
    size_t used;
    size_t size;

} ObjectArray;

int consoleWidth, consoleHeigth;
HANDLE consoleHandle;
enum GameState gameState;
ObjectArray objectArray;

void GameInit();
void GameConsoleInit (int *width, int *heigth, HANDLE *handle);
void BuildBorders(int width, int heigth);
void PrintStringOnPosition(char* s, int color, int x, int y);
void PrintCharOnPosition (char c, int color, int x, int y);
void BuildMainMenu(enum GameState state);
void Clear(int width, int heigth);
void GenerateWorld (int width, int heigth);
void ObjectArrayInit (ObjectArray *objectArray, size_t initialSize);
void InsertObjectOnArray (ObjectArray *objectArray, Object object);
void FreeObjectArray (ObjectArray *objectArray);
void Render ();
void PlayerControl ();