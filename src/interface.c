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

Text CreateText(char *content,
                unsigned short color,
                Vector2D position,
                Vector2D consoleSize,
                int update,
                enum Alignment alignment)
{
    Text text = {
        .contentSize = (int)strlen(content),
        .color = color,
        .update = update};

    text.content = malloc(sizeof(char) * (text.contentSize + 1));
    memcpy(text.content, content, sizeof(char) * (text.contentSize + 1));

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
                    int update,
                    enum Alignment alignment)
{
    Button button = {
        .contentSize = (int)strlen(content),
        .color = color,
        .event = event,
        .update = update};

    button.content = malloc(sizeof(char) * (button.contentSize + 1));
    memcpy(button.content, content, sizeof(char) * (button.contentSize + 1));

    button.position = CalculateAlignedPosition(button.content, position, consoleSize, alignment);

    return button;
}

void FreeButton(Button *button)
{
    free(button->content);
}

/*  Define o comportamento de uma interface.
*/
void InterfaceBehaviour(EventStateContext *eventStateContextPtr, Interface *interfacePtr, int *interfaceKeyLockPtr)
{
    if (!*interfaceKeyLockPtr)
    {
        if (GetKeyState(VK_RETURN) & 0x8000) // Enter
        {
            if (interfacePtr->buttonCount > 0)
            {
                // Adiciona o evento do botão na lista de eventos caso o botão seja válido
                if (interfacePtr->buttons[interfacePtr->selectedButton].event != IDLE)
                {
                    pthread_mutex_lock(&eventStateContextPtr->eventMutex);
                    if (eventStateContextPtr->event == IDLE)
                    {
                        eventStateContextPtr->event = interfacePtr->buttons[interfacePtr->selectedButton].event;
                    }
                    pthread_mutex_unlock(&eventStateContextPtr->eventMutex);
                    interfacePtr->update = 1;
                }

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
                interfacePtr->buttons[interfacePtr->selectedButton].update = 1;
                interfacePtr->buttons[interfacePtr->selectedButton + 1].update = 1;
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
                interfacePtr->buttons[interfacePtr->selectedButton].update = 1;
                interfacePtr->buttons[interfacePtr->selectedButton - 1].update = 1;
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
                    interfacePtr->update = 1;
                    break;
                case START_MENU:

                    eventStateContextPtr->event = UI_RETURN;
                    interfacePtr->update = 1;
                    break;
                case GAMEPLAY:

                    eventStateContextPtr->event = UI_PAUSE;
                    break;
                case PAUSE:

                    eventStateContextPtr->event = UI_RESUME;
                    interfacePtr->update = 1;
                    break;
                case GAMEOVER:

                    eventStateContextPtr->event = UI_RETURN;
                    interfacePtr->update = 1;
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

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->mainMenu);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->mainMenu, &interfaceCtxPtr->interfaceKeyLock);
        break;
    case INFO_MENU:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->infoMenu);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->infoMenu, &interfaceCtxPtr->interfaceKeyLock);
        break;
    case START_MENU:
        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->startMenu);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->startMenu, &interfaceCtxPtr->interfaceKeyLock);
        break;
    case GAMEPLAY:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->gameplay);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->gameplay, &interfaceCtxPtr->interfaceKeyLock);
        break;
    case PAUSE:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->pause);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->pause, &interfaceCtxPtr->interfaceKeyLock);
        break;
    case GAMEOVER:

        RenderInterface(consoleCtxPtr, &interfaceCtxPtr->gameover);
        InterfaceBehaviour(eventStateContextPtr, &interfaceCtxPtr->gameover, &interfaceCtxPtr->interfaceKeyLock);
        break;
    default:
        break;
    }
}

/*  Renderiza a interface.
*/
void RenderInterface(ConsoleContext *consoleCtxPtr, Interface *interfacePtr)
{
    // Limpa a tela caso toda a interface deva ser atualizada
    if (interfacePtr->update)
    {
        ClearOutput(consoleCtxPtr);
    }

    // Renderiza cada texto
    for (int i = 0; i < interfacePtr->textCount; i++)
    {
        if (interfacePtr->update || interfacePtr->texts[i].update)
        {
            PrintStringOnPosition(consoleCtxPtr,
                                  interfacePtr->texts[i].content,
                                  interfacePtr->texts[i].color,
                                  interfacePtr->texts[i].position);

            interfacePtr->texts[i].update = 0;
        }
    }

    // Renderiza cada botão
    for (int i = 0; i < interfacePtr->buttonCount; i++)
    {
        if (interfacePtr->update || interfacePtr->buttons[i].update)
        {
            PrintStringOnPosition(consoleCtxPtr,
                                  interfacePtr->buttons[i].content,
                                  interfacePtr->buttons[i].color,
                                  interfacePtr->buttons[i].position);

            interfacePtr->buttons[i].update = 0;
        }
    }

    // Move o cursor para o canto da tela
    SetCursorPosition(consoleCtxPtr, CreateVector2D(consoleCtxPtr->size.x - 1, consoleCtxPtr->size.y - 1));

    interfacePtr->update = 0; // Reseta o estado de atualização da interface

    WriteOutput(consoleCtxPtr);
}

Interface BuildMainMenuInterface(Vector2D consoleSize)
{
    Interface mainMenu = {

        .textCount = 2,
        .buttonCount = 3,
        .selectedButton = 0,
        .update = 1};

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
                                   0,
                                   TOP);

    mainMenu.texts[1] = CreateText(VERSION, 0x07, CreateVector2D(0, -1), consoleSize, 0, BOTTOM);

    mainMenu.buttons[0] = CreateButton("Start", 0x0C, CreateVector2D(0, 0), consoleSize, UI_START, 0, CENTER);
    mainMenu.buttons[1] = CreateButton("Info", 0x07, CreateVector2D(0, 2), consoleSize, UI_INFO, 0, CENTER);
    mainMenu.buttons[2] = CreateButton("Quit", 0x07, CreateVector2D(0, 4), consoleSize, UI_QUIT, 0, CENTER);

    return  mainMenu;
}

Interface BuildInfoInterface(Vector2D consoleSize)
{
    Interface infoMenu = {

        .textCount = 4,
        .buttonCount = 1,
        .selectedButton = 0,
        .update = 1};

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
                                   0,
                                   TOP);

    infoMenu.texts[1] = CreateText("Adaptation of my first game that was created in 2019-03-19",
                                    0x07,
                                    CreateVector2D(0, 0),
                                    consoleSize,
                                    0,
                                    CENTER);

    infoMenu.texts[2] = CreateText("Written by Eric (ErFer7): https://github.com/ErFer7/Survive",
                                   0x07,
                                   CreateVector2D(0, 1),
                                   consoleSize,
                                   0,
                                   CENTER);

    infoMenu.texts[3] = CreateText("Use the arrows to move and X to run",
                                   0x07,
                                   CreateVector2D(0, 3),
                                   consoleSize,
                                   0,
                                   CENTER);

    infoMenu.buttons[0] = CreateButton("Back", 0x0C, CreateVector2D(0, 5), consoleSize, UI_RETURN, 0, CENTER);

    return infoMenu;
}

Interface BuildStartInterface(Vector2D consoleSize)
{
    Interface startMenu = {

        .textCount = 3,
        .buttonCount = 6,
        .selectedButton = 1,
        .update = 1};

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
                                    0,
                                    TOP);

    startMenu.texts[1] = CreateText("Choose your game mode and world size",
                                    0x07,
                                    CreateVector2D(0, 0),
                                    consoleSize,
                                    1,
                                    CENTER);

    startMenu.texts[2] = CreateText("Large worlds can use a lot of memory!",
                                    0x0E,
                                    CreateVector2D(0, 1),
                                    consoleSize,
                                    1,
                                    CENTER);

    startMenu.buttons[0] = CreateButton("Small  ", 0x07, CreateVector2D(0, 3), consoleSize, UI_START_SMALL, 0, CENTER);
    startMenu.buttons[1] = CreateButton("Regular", 0x0C, CreateVector2D(0, 4), consoleSize, UI_START_REGULAR, 0, CENTER);
    startMenu.buttons[2] = CreateButton("Large  ", 0x07, CreateVector2D(0, 5), consoleSize, UI_START_LARGE, 0, CENTER);
    startMenu.buttons[3] = CreateButton("MEGA   ", 0x07, CreateVector2D(0, 6), consoleSize, UI_START_MEGA, 0, CENTER);
    startMenu.buttons[4] = CreateButton("Classic", 0x07, CreateVector2D(0, 7), consoleSize, UI_START_CLASSIC, 0, CENTER);
    startMenu.buttons[5] = CreateButton("Back", 0x07, CreateVector2D(0, 9), consoleSize, UI_RETURN, 0, CENTER);

    return startMenu;
}

Interface BuildGameplayInterface(Vector2D consoleSize)
{
    Interface gameplay = {

        .textCount = 4,
        .buttonCount = 0,
        .selectedButton = 0,
        .update = 1};

    gameplay.texts = malloc(sizeof(Text) * gameplay.textCount);

    gameplay.texts[0] = CreateText("FPS: 0000.000", 0x07, CreateVector2D(0, 0), consoleSize, 1, BOTTOM_LEFT);
    gameplay.texts[1] = CreateText("BLT: 0000.000 ms", 0x07, CreateVector2D(15, 0), consoleSize, 1, BOTTOM_LEFT);
    gameplay.texts[2] = CreateText("PLT: 0000.000 ms", 0x07, CreateVector2D(33, 0), consoleSize, 1, BOTTOM_LEFT);
    gameplay.texts[3] = CreateText("Score: 0000000000", 0x07, CreateVector2D(-25, 0), consoleSize, 1, BOTTOM_RIGHT);

    return gameplay;
}

Interface BuildPauseInterface(Vector2D consoleSize)
{
    Interface pause = {

        .textCount = 1,
        .buttonCount = 3,
        .selectedButton = 0,
        .update = 1};

    pause.texts = malloc(sizeof(Text) * pause.textCount);
    pause.buttons = malloc(sizeof(Button) * pause.buttonCount);

    pause.texts[0] = CreateText("$$$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$$\\ $$$$$$$\\  \n$$  __"
                                "$$\\ $$  __$$\\ $$ |  $$ |$$  __$$\\ $$  _____|$$  __$$\\ \n$$ |  $$ |$$ / "
                                " $$ |$$ |  $$ |$$ /  \\__|$$ |      $$ |  $$ |\n$$$$$$$  |$$$$$$$$ |$$ |  $"
                                "$ |\\$$$$$$\\  $$$$$\\    $$ |  $$ |\n$$  ____/ $$  __$$ |$$ |  $$ | \\____"
                                "$$\\ $$  __|   $$ |  $$ |\n$$ |      $$ |  $$ |$$ |  $$ |$$\\   $$ |$$ |   "
                                "   $$ |  $$ |\n$$ |      $$ |  $$ |\\$$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$"
                                "  |\n\\__|      \\__|  \\__| \\______/  \\______/ \\________|\\_______/ ",
                                0x07,
                                CreateVector2D(0, 4),
                                consoleSize,
                                0,
                                TOP);

    pause.buttons[0] = CreateButton("Resume", 0x0C, CreateVector2D(0, 0), consoleSize, UI_RESUME, 0, CENTER);
    pause.buttons[1] = CreateButton("Restart", 0x07, CreateVector2D(0, 2), consoleSize, UI_RESTART, 0, CENTER);
    pause.buttons[2] = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, 0, CENTER);

    return pause;
}

Interface BuildGameoverInterface(Vector2D consoleSize)
{
    Interface gameover = {

        .textCount = 2,
        .buttonCount = 2,
        .selectedButton = 0,
        .update = 1};

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
                                   0,
                                   TOP);

    gameover.texts[1] = CreateText("Score: 0000000000", 0x07, CreateVector2D(0, 0), consoleSize, 0, CENTER);

    gameover.buttons[0] = CreateButton("Restart", 0x0C, CreateVector2D(0, 2), consoleSize, UI_RESTART, 0, CENTER);
    gameover.buttons[1] = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, 0, CENTER);

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
