#pragma once

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/graphics.h"

#define MAX_TEXT_STRLEN 1024
#define MAX_BUTTON_STRLEN 32
#define MAX_TEXTS 4
#define MAX_BUTTONS 16
#define VERSION "2.17-dev"

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
    char content[MAX_TEXT_STRLEN];
    unsigned short color;
    Vector2D position;
    int update;

} Text;

// Botão
typedef struct
{
    char content[MAX_BUTTON_STRLEN];
    unsigned short color;
    Vector2D position;
    enum Event event;
    int update;

} Button;

// Interface
typedef struct
{
    Text texts[MAX_TEXTS];
    Button buttons[MAX_BUTTONS];
    int selectedButton;
    int update;

} Interface;

typedef struct
{
    Interface mainMenu;
    Interface startMenu;
    Interface infoMenu;
    Interface gameplay;
    Interface pause;
    Interface gameover;
    int interfaceKeyLock;
} InterfaceContext;

Vector2D CalculateAlignedPosition(char *string, Vector2D position, Vector2D consoleSize, enum Alignment alignment);
Text CreateText(char content[MAX_TEXT_STRLEN],
                unsigned short color,
                Vector2D position,
                Vector2D consoleSize,
                int update,
                enum Alignment alignment);
Button CreateButton(char content[MAX_BUTTON_STRLEN],
                    unsigned short color,
                    Vector2D position,
                    Vector2D consoleSize,
                    enum Event event,
                    int update,
                    enum Alignment alignment);
void InterfaceBehaviour(EventStateContext *eventStateContextPtr, Interface *interfacePtr, int *interfaceKeyLockPtr);
void UpdateInterfaces(EventStateContext *eventStateContextPtr,
                      InterfaceContext *interfaceCtxPtr,
                      ConsoleContext *consoleCtxPtr);
void RenderInterface(ConsoleContext *consoleCtxPtr, Interface *interfacePtr);
Interface BuildMainMenuInterface(Vector2D consoleSize);
Interface BuildInfoInterface(Vector2D consoleSize);
Interface BuildStartInterface(Vector2D consoleSize);
Interface BuildGameplayInterface(Vector2D consoleSize);
Interface BuildPauseInterface(Vector2D consoleSize);
Interface BuildGameoverInterface(Vector2D consoleSize);
void InitInterfaceContext(InterfaceContext *interfaceCtxPtr, Vector2D consoleSize);
