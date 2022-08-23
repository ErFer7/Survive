#pragma once

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/graphics.h"

#define VERSION "2.18"

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

typedef struct
{
    char *content;
    int contentSize;
    unsigned short color;
    Vector2D position;
    int update;
} Text;

typedef struct
{
    char *content;
    int contentSize;
    unsigned short color;
    Vector2D position;
    enum Event event;
    int update;
} Button;

typedef struct
{
    Text *texts;
    int textCount;
    Button *buttons;
    int buttonCount;
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

void InitInterfaceContext(InterfaceContext *interfaceCtxPtr, Vector2D consoleSize);
void FreeInterfaceContext(InterfaceContext *interfaceCtxPtr);
Vector2D CalculateAlignedPosition(char *string, Vector2D position, Vector2D consoleSize, enum Alignment alignment);
Text CreateText(char *content, unsigned short color, Vector2D position, Vector2D consoleSize, enum Alignment alignment);
void FreeText(Text *text);
Button CreateButton(char *content,
                    unsigned short color,
                    Vector2D position,
                    Vector2D consoleSize,
                    enum Event event,
                    enum Alignment alignment);
void FreeButton(Button *button);
void SetGameplayTextd(Text *textPtr, double value);
void SetGameplayText(Text *textPtr, int value);
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
void FreeInterface(Interface *interfacePtr);
