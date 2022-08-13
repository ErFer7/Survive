#pragma once

#include "../include/core.h"

#define MAX_TEXT_STRLEN 1024
#define MAX_BUTTON_STRLEN 32
#define MAX_TEXTS 4
#define MAX_BUTTONS 16
#define VERSION "2.16"

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
    unsigned short color;          // 1 B
    int position[2];               // 4 B
    int update;                    // 1 B

} Text;

// Botão
typedef struct
{
    char content[MAX_BUTTON_STRLEN]; // 9 B
    unsigned short color;            // 1 B
    int position[2];                 // 4 B
    enum Event event;                // (1 a 4) B
    int update;                      // 1 B

} Button;

// Interface
typedef struct
{
    Text texts[MAX_TEXTS];       // 2178 B
    Button buttons[MAX_BUTTONS]; // (48 a 57) B
    int selectedButton;          // 4 B
    int update;                  // 1 B

} Interface;

extern Interface mainMenu;
extern Interface startMenu;
extern Interface infoMenu;
extern Interface gameplay;
extern Interface pause;
extern Interface gameover;
extern int interfaceKeyLock;

void CalculateAlignedPosition(char* string, int *x, int *y, enum Alignment alignment);
Text CreateText(char content[MAX_TEXT_STRLEN],
                unsigned short color,
                int x,
                int y,
                int update,
                enum Alignment alignment);
Button CreateButton(char content[MAX_BUTTON_STRLEN],
                    unsigned short color,
                    int x,
                    int y,
                    enum Event event,
                    int update,
                    enum Alignment alignment);
void InterfaceBehaviour(Interface *interface_);
void UpdateInterfaces();
void RenderInterface(Interface *interface_);
void BuildMainMenuInterface();
void BuildInfoInterface();
void BuildStartInterface();
void BuildGameplayInterface();
void BuildPauseInterface();
void BuildGameoverInterface();
void InitInterface();
