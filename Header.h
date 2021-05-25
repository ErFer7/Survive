#pragma once

#include <Windows.h>
#include <stdint.h>

#define MAX_TEXT_STRLEN 720
#define MAX_BUTTON_STRLEN 9
#define MAX_TEXTS 3
#define MAX_BUTTONS 3
#define MAX_EVENTS 2

enum Event
{
    IDLE,
    UI_PLAY,
    UI_INFO,
    UI_QUIT,
    UI_PAUSE,
    UI_RESUME,
    UI_RESTART,
    UI_RETURN,
    GM_GAMEOVER
};

enum State
{
    MAIN_MENU,
    INFO_MENU,
    GAMEPLAY,
    PAUSE,
    GAMEOVER,
    EXIT
};

enum Alignment
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT
};

enum ObjectType
{
    EMPTY,
    WALL,
    PLAYER,
    COIN,
    ENEMY
};

// Texto: ~726 B
typedef struct
{
    char content[MAX_TEXT_STRLEN]; // 720 B
    uint8_t color;                 // 1 B
    int16_t position[2];           // 4 B
    uint8_t update;                // 1 B

} Text;

// Botão: ~16 a ~19 B
typedef struct
{
    char content[MAX_BUTTON_STRLEN]; // 9 B
    uint8_t color;                   // 1 B
    int16_t position[2];             // 4 B
    enum Event event;                // (1 a 4) B
    uint8_t update;                  // 1 B

} Button;

// Interface: ~2235 B a ~2244 B
typedef struct
{
    Text texts[MAX_TEXTS];       // 2178 B
    Button buttons[MAX_BUTTONS]; // (48 a 57) B
    float selectedButton;        // 4 B
    float selectionSpeed;        // 4 B
    uint8_t update;              // 1 B

} Interface;

// Objeto: ~25 B a ~28 B
typedef struct
{
    uint16_t id;          // 2 B
    char c;               // 1 B
    uint8_t color;        // 1 B
    float velocity[2];    // 8 B
    float position[2];    // 8 B
    float speed;          // 4 B
    enum ObjectType type; // (1 a 4) B

} Object;

// Matriz: Esperado: 90.05 kB, Máximo: 1.82 MB
typedef struct
{
    Object *matrix;    // Max: (1.625 a 1.82) MB (Alocado na posição)
    Object *playerPtr; // (25 a 28) B (Alocado na posição)
    Object *coinPtr;   // (25 a 28) B (Alocado na posição)
    uint8_t width;     // 1 B
    uint8_t height;    // 1 B

} ObjectMatrix;

uint8_t consoleWidth, consoleHeight;
HANDLE consoleHandle;
enum State state;
enum Event events[MAX_EVENTS];

Interface mainMenu;
Interface infoMenu;
Interface gameplay;
Interface pause;
Interface gameover;

ObjectMatrix objectMatrix;
ObjectMatrix oldObjectMatrix;
unsigned int tick;
uint16_t score;
uint16_t idCount;
uint8_t renderAll;

void GameInit(unsigned int t);
void PrintCharOnPosition(char c, uint8_t color, uint8_t x, uint8_t y);
void PrintStringOnPosition(char *s, uint8_t color, uint8_t x, uint8_t y);
void BuildBorders();
void CalculateAlignedPosition(int16_t *x, int16_t *y, uint8_t sizeX, uint8_t sizeY, enum Alignment alignment);
void ObjectMatrixInit(ObjectMatrix *objectMatrix, uint8_t width, uint8_t height);
void InsertObjectOnMatrix(ObjectMatrix *objectMatrix, Object object, uint8_t x, uint8_t y);
void MoveObjectOnMatrix(ObjectMatrix *objectMatrix, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
Object *GetObjectPtrFromMatrix(ObjectMatrix *objectMatrix, uint8_t x, uint8_t y);
void FreeObjectMatrix(ObjectMatrix *objectMatrix);
void InterfaceBehaviour(Interface *interfaceIn);
void RenderInterface(Interface *interfaceIn);
void UpdateInterfaces();
void Clear();
void GenerateWorld();
void UpdateObjectBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(uint8_t x, uint8_t y);
void Render();
float Tick(double elapsedTime);
void UpdatePhysics();
void UpdateMatrices();
float Randomf(int min, int max);