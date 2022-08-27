#include "../include/interface.h"

#include <stdio.h>
#include <pthread.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"

void InitInterfaceContext(InterfaceContext *interfaceCtxPtr, Vector2D consoleSize)
{
    interfaceCtxPtr->mainMenu = BuildMainMenuInterface(consoleSize);
    interfaceCtxPtr->infoMenu = BuildInfoInterface(consoleSize);
    interfaceCtxPtr->startMenu = BuildStartInterface(consoleSize);
    interfaceCtxPtr->gameplay = BuildGameplayInterface(consoleSize);
    interfaceCtxPtr->pause = BuildPauseInterface(consoleSize);
    interfaceCtxPtr->gameover = BuildGameoverInterface(consoleSize);

    interfaceCtxPtr->clearBackground = 1;
    interfaceCtxPtr->interfaceKeyLock = 0;
}

void FreeInterfaceContext(InterfaceContext *interfaceCtxPtr)
{
    FreeInterface(&interfaceCtxPtr->mainMenu);
    FreeInterface(&interfaceCtxPtr->infoMenu);
    FreeInterface(&interfaceCtxPtr->startMenu);
    FreeInterface(&interfaceCtxPtr->gameplay);
    FreeInterface(&interfaceCtxPtr->pause);
    FreeInterface(&interfaceCtxPtr->gameover);
}

Vector2D CalculateAlignedPosition(char *string, Vector2D position, Vector2D consoleSize, enum Alignment alignment)
{
    /* Calcula a posição com base no alinhamento e retorna.
     */

    Vector2D calculatedPosition = position;
    int width = 0;
    int height = 0;
    int lineWidth = 0;
    int i = 0;
    char c;

    while (1)
    {
        c = string[i];

        if (c != '\0')
        {
            if (c != '\n')
            {
                lineWidth++;

                if (lineWidth > width)
                {
                    width = lineWidth;
                }
            }
            else
            {
                lineWidth = 0;
                height++;
            }

            i++;
        }
        else
        {
            break;
        }
    }

    switch (alignment)
    {
    case TOP:

        calculatedPosition.x += (consoleSize.x - width) / 2;
        break;
    case TOP_RIGHT:

        calculatedPosition.x += (consoleSize.x + width / 2) - 1;
        break;
    case LEFT:

        calculatedPosition.y += (consoleSize.y - height) / 2;
        break;
    case CENTER:

        calculatedPosition.x += (consoleSize.x - width) / 2;
        calculatedPosition.y += (consoleSize.y - height) / 2;
        break;
    case RIGHT:

        calculatedPosition.x += (consoleSize.x + width / 2) - 1;
        calculatedPosition.y += (consoleSize.y - height) / 2;
        break;
    case BOTTOM_LEFT:

        calculatedPosition.y += (consoleSize.y + height / 2) - 1;
        break;
    case BOTTOM:

        calculatedPosition.x += (consoleSize.x - width) / 2;
        calculatedPosition.y += (consoleSize.y + height / 2) - 1;
        break;
    case BOTTOM_RIGHT:

        calculatedPosition.x += (consoleSize.x + width / 2) - 1;
        calculatedPosition.y += (consoleSize.y + height / 2) - 1;
        break;
    default:
        break;
    }

    return calculatedPosition;
}

Text CreateText(char *content, unsigned short color, Vector2D position, Vector2D consoleSize, enum Alignment alignment)
{
    Text text = {
        .contentSize = (int)strlen(content) + 1,
        .color = color};

    text.content = malloc(sizeof(char) * text.contentSize);
    memcpy(text.content, content, sizeof(char) * text.contentSize);

    text.position = CalculateAlignedPosition(text.content, position, consoleSize, alignment);

    return text;
}

void FreeText(Text *text)
{
    free(text->content);
}

Button CreateButton(char *content,
                    unsigned short color,
                    Vector2D position,
                    Vector2D consoleSize,
                    enum Event event,
                    enum Alignment alignment)
{
    Button button = {
        .contentSize = (int)strlen(content) + 1,
        .color = color,
        .event = event};

    button.content = malloc(sizeof(char) * button.contentSize);
    memcpy(button.content, content, sizeof(char) * button.contentSize);

    button.position = CalculateAlignedPosition(button.content, position, consoleSize, alignment);

    return button;
}

void FreeButton(Button *button)
{
    free(button->content);
}

void SetGameplayTextd(Text *textPtr, double value)
{
    if (value > 999999.999)
    {
        sprintf_s(textPtr->content, textPtr->contentSize, "> 1mi     ");
    }
    else
    {
        sprintf_s(textPtr->content, textPtr->contentSize, "%010.3f", value);
    }
}

void SetGameplayText(Text *textPtr, int value)
{
    if (value > INT_MAX)
    {
        sprintf_s(textPtr->content, textPtr->contentSize, "OVERFLOW  ");
    }
    else
    {
        sprintf_s(textPtr->content, textPtr->contentSize, "%010d", value);
    }
}

/*  Define o comportamento de uma interface.
*/
void InterfaceBehaviour(EventStateContext *eventStateContextPtr,
                        Interface *interfacePtr,
                        int *clearBackgroundPtr,
                        int *interfaceKeyLockPtr)
{
    if (!*interfaceKeyLockPtr)
    {
        if (GetKeyState(VK_RETURN) & 0x8000) // Enter
        {
            if (interfacePtr->buttonCount > 0)
            {
                pthread_mutex_lock(&eventStateContextPtr->eventMutex);
                if (eventStateContextPtr->event == IDLE)
                {
                    eventStateContextPtr->event = interfacePtr->buttons[interfacePtr->selectedButton].event;
                }
                pthread_mutex_unlock(&eventStateContextPtr->eventMutex);

                *clearBackgroundPtr = 1;
                *interfaceKeyLockPtr = 1;
            }
        }
        else if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
        {
            // Seleciona o botão superior caso possível
            if (interfacePtr->selectedButton > 0)
            {
                interfacePtr->selectedButton--;
                interfacePtr->buttons[interfacePtr->selectedButton].color = 0x0C;
                interfacePtr->buttons[interfacePtr->selectedButton + 1].color = 0x07;
            }

            *interfaceKeyLockPtr = 1;
        }
        else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
        {
            // Seleciona o botão inferior caso possível
            if (interfacePtr->selectedButton < interfacePtr->buttonCount - 1 &&
                interfacePtr->buttons[interfacePtr->selectedButton + 1].event != IDLE)
            {
                interfacePtr->selectedButton++;
                interfacePtr->buttons[interfacePtr->selectedButton].color = 0x0C;
                interfacePtr->buttons[interfacePtr->selectedButton - 1].color = 0x07;
            }

            *interfaceKeyLockPtr = 1;
        }
        else if (GetKeyState(VK_ESCAPE) & 0x8000) // Esc
        {
            // Adiciona o evento correspondente ao estado na lista de eventos
            pthread_mutex_lock(&eventStateContextPtr->eventMutex);
            if (eventStateContextPtr->event == IDLE)
            {
                switch (eventStateContextPtr->state)
                {
                case MAIN_MENU:

                    eventStateContextPtr->event = UI_QUIT;
                    break;
                case INFO_MENU:

                    eventStateContextPtr->event = UI_RETURN;
                    *clearBackgroundPtr = 1;
                    break;
                case START_MENU:

                    eventStateContextPtr->event = UI_RETURN;
                    *clearBackgroundPtr = 1;
                    break;
                case GAMEPLAY:

                    eventStateContextPtr->event = UI_PAUSE;
                    break;
                case PAUSE:

                    eventStateContextPtr->event = UI_RESUME;
                    *clearBackgroundPtr = 1;
                    break;
                case GAMEOVER:

                    eventStateContextPtr->event = UI_RETURN;
                    *clearBackgroundPtr = 1;
                    break;
                default:
                    break;
                }
            }
            pthread_mutex_unlock(&eventStateContextPtr->eventMutex);

            *interfaceKeyLockPtr = 1;
        }
    }

    if (*interfaceKeyLockPtr && !(GetKeyState(VK_RETURN) & 0x8000 ||
                                  GetKeyState(VK_ESCAPE) & 0x8000 ||
                                  GetKeyState(VK_UP) & 0x8000 ||
                                  GetKeyState(VK_DOWN) & 0x8000))
    {
        *interfaceKeyLockPtr = 0;
    }
}

/*  Atualiza todas as interfaces com base no estado.
*/
void UpdateInterfaces(EventStateContext *eventStateContextPtr,
                      InterfaceContext *interfaceCtxPtr,
                      ConsoleContext *consoleCtxPtr)
{
    switch (eventStateContextPtr->state)
    {
    case MAIN_MENU:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->mainMenu, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->mainMenu,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    case INFO_MENU:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->infoMenu, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->infoMenu,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    case START_MENU:
        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->startMenu, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->startMenu,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    case GAMEPLAY:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->gameplay, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->gameplay,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    case PAUSE:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->pause, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->pause,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    case GAMEOVER:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->gameover, &interfaceCtxPtr->clearBackground);
        InterfaceBehaviour(eventStateContextPtr,
                           &interfaceCtxPtr->gameover,
                           &interfaceCtxPtr->clearBackground,
                           &interfaceCtxPtr->interfaceKeyLock);
        break;
    default:
        break;
    }
}

/*  Renderiza a interface.
*/
void RenderInterface(ConsoleContext *consoleCtxPtr, Interface *interfacePtr, int *clearBackgroundPtr)
{
    // Limpa a tela caso toda a interface deva ser atualizada
    if (*clearBackgroundPtr)
    {
        ClearOutput(consoleCtxPtr);
    }

    // Renderiza cada texto
    for (int i = 0; i < interfacePtr->textCount; i++)
    {
        PrintStringOnPosition(consoleCtxPtr,
                              interfacePtr->texts[i].content,
                              interfacePtr->texts[i].color,
                              interfacePtr->texts[i].position);
    }

    // Renderiza cada botão
    for (int i = 0; i < interfacePtr->buttonCount; i++)
    {
        PrintStringOnPosition(consoleCtxPtr,
                              interfacePtr->buttons[i].content,
                              interfacePtr->buttons[i].color,
                              interfacePtr->buttons[i].position);
    }

    // Move o cursor para o canto da tela
    SetCursorPosition(consoleCtxPtr, CreateVector2D(consoleCtxPtr->size.x - 1, consoleCtxPtr->size.y - 1));

    *clearBackgroundPtr = 0; // Reseta o estado de atualização da interface

    WriteOutput(consoleCtxPtr);
}

Interface BuildMainMenuInterface(Vector2D consoleSize)
{
    Interface mainMenu = {

        .textCount = 2,
        .buttonCount = 3,
        .selectedButton = 0};

    mainMenu.texts = malloc(sizeof(Text) * mainMenu.textCount);
    mainMenu.buttons = malloc(sizeof(Button) * mainMenu.buttonCount);

    mainMenu.texts[0] = CreateText("  ______   __    __  _______   __     __  ______  __     __  ________  \n"
                                   " /      \\ |  \\  |  \\|       \\ |  \\   |  \\|      \\|  \\   |  \\|   "
                                   "     \\\n|  $$$$$$\\| $$  | $$| $$$$$$$\\| $$   | $$ \\$$$$$$| $$   | $$|"
                                   " $$$$$$$$\n| $$___\\$$| $$  | $$| $$__| $$| $$   | $$  | $$  | $$   | $$|"
                                   " $$__    \n \\$$    \\ | $$  | $$| $$    $$ \\$$\\ /  $$  | $$   \\$$\\ /"
                                   "  $$| $$  \\   \n _\\$$$$$$\\| $$  | $$| $$$$$$$\\  \\$$\\  $$   | $$    "
                                   "\\$$\\  $$ | $$$$$   \n|  \\__| $$| $$__/ $$| $$  | $$   \\$$ $$   _| $$_"
                                   "    \\$$ $$  | $$_____ \n \\$$    $$ \\$$    $$| $$  | $$    \\$$$   |   "
                                   "$$ \\    \\$$$   | $$     \\\n  \\$$$$$$   \\$$$$$$  \\$$   \\$$     \\$ "
                                   "    \\$$$$$$     \\$     \\$$$$$$$$",
                                   0x0C,
                                   CreateVector2D(0, 4),
                                   consoleSize,
                                   TOP);

    mainMenu.texts[1] = CreateText(VERSION, 0x07, CreateVector2D(0, -1), consoleSize, BOTTOM);

    mainMenu.buttons[0] = CreateButton("Start", 0x0C, CreateVector2D(0, 0), consoleSize, UI_START, CENTER);
    mainMenu.buttons[1] = CreateButton("Info", 0x07, CreateVector2D(0, 2), consoleSize, UI_INFO, CENTER);
    mainMenu.buttons[2] = CreateButton("Quit", 0x07, CreateVector2D(0, 4), consoleSize, UI_QUIT, CENTER);

    return  mainMenu;
}

Interface BuildInfoInterface(Vector2D consoleSize)
{
    Interface infoMenu = {

        .textCount = 4,
        .buttonCount = 1,
        .selectedButton = 0};

    infoMenu.texts = malloc(sizeof(Text) * infoMenu.textCount);
    infoMenu.buttons = malloc(sizeof(Button) * infoMenu.buttonCount);

    infoMenu.texts[0] = CreateText(" ______  __    __  ________   ______  \n|      \\|  \\  |  \\|        \\ /   "
                                   "   \\ \n \\$$$$$$| $$\\ | $$| $$$$$$$$|  $$$$$$\\\n  | $$  | $$$\\| $$| $$__ "
                                   "   | $$  | $$\n  | $$  | $$$$\\ $$| $$  \\   | $$  | $$\n  | $$  | $$\\$$ $$|"
                                   " $$$$$   | $$  | $$\n _| $$_ | $$ \\$$$$| $$      | $$__/ $$\n|   $$ \\| $$"
                                   "  \\$$$| $$       \\$$    $$\n \\$$$$$$ \\$$   \\$$ \\$$        \\$$$$$$ ",
                                   0x0A,
                                   CreateVector2D(0, 4),
                                   consoleSize,
                                   TOP);

    infoMenu.texts[1] = CreateText("Adaptation of my first game that was created in 2019-03-19",
                                    0x07,
                                    CreateVector2D(0, 0),
                                    consoleSize,
                                    CENTER);

    infoMenu.texts[2] = CreateText("Written by Eric (ErFer7): https://github.com/ErFer7/Survive",
                                   0x07,
                                   CreateVector2D(0, 1),
                                   consoleSize,
                                   CENTER);

    infoMenu.texts[3] = CreateText("Use the arrows to move and X to run",
                                   0x07,
                                   CreateVector2D(0, 3),
                                   consoleSize,
                                   CENTER);

    infoMenu.buttons[0] = CreateButton("Back", 0x0C, CreateVector2D(0, 5), consoleSize, UI_RETURN, CENTER);

    return infoMenu;
}

Interface BuildStartInterface(Vector2D consoleSize)
{
    Interface startMenu = {

        .textCount = 3,
        .buttonCount = 6,
        .selectedButton = 1};

    startMenu.texts = malloc(sizeof(Text) * startMenu.textCount);
    startMenu.buttons = malloc(sizeof(Button) * startMenu.buttonCount);

    startMenu.texts[0] = CreateText("  ______  ________   ______   _______  ________\n /      \\|        \\ /     "
                                    " \\ |       \\|        \\\n|  $$$$$$\\\\$$$$$$$$|  $$$$$$\\| $$$$$$$\\\\$$$$$"
                                    "$$$\n| $$___\\$$  | $$   | $$__| $$| $$__| $$  | $$\n \\$$    \\   | $$   | $"
                                    "$    $$| $$    $$  | $$\n _\\$$$$$$\\  | $$   | $$$$$$$$| $$$$$$$\\  | $$\n| "
                                    " \\__| $$  | $$   | $$  | $$| $$  | $$  | $$\n \\$$    $$  | $$   | $$  | $$|"
                                    " $$  | $$  | $$\n  \\$$$$$$    \\$$    \\$$   \\$$ \\$$   \\$$   \\$$",
                                    0x0C,
                                    CreateVector2D(0, 4),
                                    consoleSize,
                                    TOP);

    startMenu.texts[1] = CreateText("Choose your game mode and world size",
                                    0x07,
                                    CreateVector2D(0, 0),
                                    consoleSize,
                                    CENTER);

    startMenu.texts[2] = CreateText("Large worlds can use a lot of memory!",
                                    0x0E,
                                    CreateVector2D(0, 1),
                                    consoleSize,
                                    CENTER);

    startMenu.buttons[0] = CreateButton("Small  ", 0x07, CreateVector2D(0, 3), consoleSize, UI_START_SMALL, CENTER);
    startMenu.buttons[1] = CreateButton("Regular", 0x0C, CreateVector2D(0, 4), consoleSize, UI_START_REGULAR, CENTER);
    startMenu.buttons[2] = CreateButton("Large  ", 0x07, CreateVector2D(0, 5), consoleSize, UI_START_LARGE, CENTER);
    startMenu.buttons[3] = CreateButton("MEGA   ", 0x07, CreateVector2D(0, 6), consoleSize, UI_START_MEGA, CENTER);
    startMenu.buttons[4] = CreateButton("Classic", 0x07, CreateVector2D(0, 7), consoleSize, UI_START_CLASSIC, CENTER);
    startMenu.buttons[5] = CreateButton("Back", 0x07, CreateVector2D(0, 9), consoleSize, UI_RETURN, CENTER);

    return startMenu;
}

Interface BuildGameplayInterface(Vector2D consoleSize)
{
    Interface gameplay = {

        .textCount = 6,
        .buttonCount = 0,
        .selectedButton = 0};

    gameplay.texts = malloc(sizeof(Text) * gameplay.textCount);

    gameplay.texts[0] = CreateText("FPS:", 0x07, CreateVector2D(0, 0), consoleSize, BOTTOM_LEFT);
    gameplay.texts[1] = CreateText("000000.000", 0x07, CreateVector2D(5, 0), consoleSize, BOTTOM_LEFT);
    gameplay.texts[2] = CreateText("UPS:", 0x07, CreateVector2D(17, 0), consoleSize, BOTTOM_LEFT);
    gameplay.texts[3] = CreateText("000000.000", 0x07, CreateVector2D(22, 0), consoleSize, BOTTOM_LEFT);
    gameplay.texts[4] = CreateText("Score:", 0x07, CreateVector2D(-21, 0), consoleSize, BOTTOM_RIGHT);
    gameplay.texts[5] = CreateText("0000000000", 0x07, CreateVector2D(-16, 0), consoleSize, BOTTOM_RIGHT);

    return gameplay;
}

Interface BuildPauseInterface(Vector2D consoleSize)
{
    Interface pause = {

        .textCount = 3,
        .buttonCount = 3,
        .selectedButton = 0};

    pause.texts = malloc(sizeof(Text) * pause.textCount);
    pause.buttons = malloc(sizeof(Button) * pause.buttonCount);

    pause.texts[0] = CreateText("----------------------------------------------------------------------\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "----------------------------------------------------------------------",
                                0x08,
                                CreateVector2D(0, 2),
                                consoleSize,
                                TOP);

    pause.texts[1] = CreateText("----------------------------------------------------------------------\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "|                                                                    |\n"
                                "----------------------------------------------------------------------",
                                0x08,
                                CreateVector2D(0, 2),
                                consoleSize,
                                CENTER);

    pause.texts[2] = CreateText("$$$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$$\\ $$$$$$$\\  \n$$  __"
                                "$$\\ $$  __$$\\ $$ |  $$ |$$  __$$\\ $$  _____|$$  __$$\\ \n$$ |  $$ |$$ / "
                                " $$ |$$ |  $$ |$$ /  \\__|$$ |      $$ |  $$ |\n$$$$$$$  |$$$$$$$$ |$$ |  $"
                                "$ |\\$$$$$$\\  $$$$$\\    $$ |  $$ |\n$$  ____/ $$  __$$ |$$ |  $$ | \\____"
                                "$$\\ $$  __|   $$ |  $$ |\n$$ |      $$ |  $$ |$$ |  $$ |$$\\   $$ |$$ |   "
                                "   $$ |  $$ |\n$$ |      $$ |  $$ |\\$$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$"
                                "  |\n\\__|      \\__|  \\__| \\______/  \\______/ \\________|\\_______/ ",
                                0x07,
                                CreateVector2D(0, 4),
                                consoleSize,
                                TOP);

    pause.buttons[0] = CreateButton("Resume", 0x0C, CreateVector2D(0, 0), consoleSize, UI_RESUME, CENTER);
    pause.buttons[1] = CreateButton("Restart", 0x07, CreateVector2D(0, 2), consoleSize, UI_RESTART, CENTER);
    pause.buttons[2] = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, CENTER);

    return pause;
}

Interface BuildGameoverInterface(Vector2D consoleSize)
{
    Interface gameover = {

        .textCount = 3,
        .buttonCount = 2,
        .selectedButton = 0};

    gameover.texts = malloc(sizeof(Text) * gameover.textCount);
    gameover.buttons = malloc(sizeof(Button) * gameover.buttonCount);

    gameover.texts[0] = CreateText(" $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\  $$$$$$\\  $$\\    $$\\ $$$$$$"
                                   "$$\\ $$$$$$$\\  \n$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|$$  __$$\\ $$ "
                                   "|   $$ |$$  _____|$$  __$$\\ \n$$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      $$"
                                   " /  $$ |$$ |   $$ |$$ |      $$ |  $$ |\n$$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$"
                                   "$$$$\\    $$ |  $$ |\\$$\\  $$  |$$$$$\\    $$$$$$$  |\n$$ |\\_$$ |$$  __$$ |$"
                                   "$ \\$$$  $$ |$$  __|   $$ |  $$ | \\$$\\$$  / $$  __|   $$  __$$< \n$$ |  $$ |"
                                   "$$ |  $$ |$$ |\\$  /$$ |$$ |      $$ |  $$ |  \\$$$  /  $$ |      $$ |  $$ |\n"
                                   "\\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\  $$$$$$  |   \\$  /   $$$$$$$$\\"
                                   " $$ |  $$ |\n \\______/ \\__|  \\__|\\__|     \\__|\\________| \\______/     "
                                   "\\_/    \\________|\\__|  \\__|",
                                   0x0C,
                                   CreateVector2D(0, 4),
                                   consoleSize,
                                   TOP);

    gameover.texts[1] = CreateText("Score:", 0x07, CreateVector2D(-6, 0), consoleSize, CENTER);
    gameover.texts[2] = CreateText("0000000000", 0x07, CreateVector2D(3, 0), consoleSize, CENTER);

    gameover.buttons[0] = CreateButton("Restart", 0x0C, CreateVector2D(0, 2), consoleSize, UI_RESTART, CENTER);
    gameover.buttons[1] = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, CENTER);

    return gameover;
}

void FreeInterface(Interface *interfacePtr)
{
    for (int i = 0; i < interfacePtr->textCount; i++)
    {
        FreeText(&interfacePtr->texts[i]);
    }

    for (int i = 0; i < interfacePtr->buttonCount; i++)
    {
        FreeButton(&interfacePtr->buttons[i]);
    }

    free(interfacePtr->texts);
    free(interfacePtr->buttons);
}
