#pragma once

#include <Windows.h>

enum GameState {MENU, INGAME, EXIT};
enum MenuState {MAIN, INFO};
enum ObjectType {WALL, PLAYER, COIN, ENEMY};

typedef struct {

    char c;
    int color;
    int position[2];
    int oldPosition[2];
    float interPosition[2];
    float speed;
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
enum MenuState menuState;
ObjectArray objectArray;
int tick;
int updateMenu;
int score;

void GameInit (int t);
void GameConsoleInit (int *width, int *heigth, HANDLE *handle);
void BuildBorders(int width, int heigth);
void PrintStringOnPosition(char* s, int color, int x, int y);
void PrintCharOnPosition (char c, int color, int x, int y);
void BuildMenu(enum MenuState state);
void Clear(int width, int heigth);
void GenerateWorld (int width, int heigth);
void ObjectArrayInit (ObjectArray *objectArray, size_t initialSize);
void InsertObjectOnArray (ObjectArray *objectArray, Object object);
void FreeObjectArray (ObjectArray *objectArray);
void Render ();
void PlayerControl ();
void EnemyBehaviour(int monsterIndex);
float Tick(double elapsedTime);
void UpdatePhysics();
int RandomInt(int min, int max);