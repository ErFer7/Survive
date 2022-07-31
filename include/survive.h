#pragma once

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

// Texto
typedef struct
{
    char content[MAX_TEXT_STRLEN]; // 720 B
    uint8_t color;                 // 1 B
    int16_t position[2];           // 4 B
    uint8_t update;                // 1 B

} Text;

// Botão
typedef struct
{
    char content[MAX_BUTTON_STRLEN]; // 9 B
    uint8_t color;                   // 1 B
    int16_t position[2];             // 4 B
    enum Event event;                // (1 a 4) B
    uint8_t update;                  // 1 B

} Button;

// Interface
typedef struct
{
    Text texts[MAX_TEXTS];       // 2178 B
    Button buttons[MAX_BUTTONS]; // (48 a 57) B
    float selectedButton;        // 4 B
    float selectionSpeed;        // 4 B
    uint8_t update;              // 1 B

} Interface;

enum State state;
enum Event events[MAX_EVENTS];

Interface mainMenu;
Interface infoMenu;
Interface gameplay;
Interface pause;
Interface gameover;

unsigned int tick;
uint16_t score;
uint16_t idCount;
uint8_t interfaceKeyLock;

void GameInit(unsigned int t, uint8_t width, uint8_t height);
void BuildBorders();
void CalculateAlignedPosition(int16_t *x, int16_t *y, uint8_t sizeX, uint8_t sizeY, enum Alignment alignment);
void InterfaceBehaviour(Interface *interfaceIn);
void RenderInterface(Interface *interfaceIn);
void UpdateInterfaces();
void GenerateWorld();
void UpdateObjectBehaviour();
void PlayerBehaviour();
void EnemyBehaviour(uint8_t x, uint8_t y);
void UpdatePhysics();
void Render();
float Tick(double elapsedTime);
float Randomf(int min, int max);
