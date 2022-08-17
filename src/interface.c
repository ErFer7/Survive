#include "../include/interface.h"

#include <stdio.h>
#include <pthread.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"

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

Text CreateText(char content[MAX_TEXT_STRLEN],
                unsigned short color,
                Vector2D position,
                Vector2D consoleSize,
                int update,
                enum Alignment alignment)
{
    Text text = {
        .color = color,
        .update = update};

    memcpy(text.content, content, sizeof(char) * MAX_TEXT_STRLEN);
    text.position = CalculateAlignedPosition(text.content, position, consoleSize, alignment);

    return text;
}

Button CreateButton(char content[MAX_BUTTON_STRLEN],
                    unsigned short color,
                    Vector2D position,
                    Vector2D consoleSize,
                    enum Event event,
                    int update,
                    enum Alignment alignment)
{
    Button button = {
        .color = color,
        .event = event,
        .update = update};

    memcpy(button.content, content, sizeof(char) * MAX_BUTTON_STRLEN);
    button.position = CalculateAlignedPosition(button.content, position, consoleSize, alignment);

    return button;
}

void InterfaceBehaviour(EventStateContext *eventStateContextPtr, Interface *interfacePtr, int *interfaceKeyLockPtr)
{
    /* Define o comportamento de uma interface.
     */
    if (!*interfaceKeyLockPtr)
    {
        if (GetKeyState(VK_RETURN) & 0x8000) // Enter
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
            if (interfacePtr->selectedButton < MAX_BUTTONS - 1 &&
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

void UpdateInterfaces(EventStateContext *eventStateContextPtr,
                      InterfaceContext *interfaceCtxPtr,
                      ConsoleContext *consoleCtxPtr)
{
    /* Atualiza todas as interfaces com base no estado.
     */

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

void RenderInterface(ConsoleContext *consoleCtxPtr, Interface *interfacePtr)
{
    /* Renderiza a interface.
     */

    // Limpa a tela caso toda a interface deva ser atualizada
    if (interfacePtr->update)
    {
        ClearOutput(consoleCtxPtr);
    }

    // Renderiza cada texto
    for (int i = 0; i < MAX_TEXTS; i++)
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
    for (int i = 0; i < MAX_BUTTONS; i++)
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
    // Título
    Text mainMenuTitle = CreateText("  ______   __    __  _______   __     __  ______  __     __  ________  \n"
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

    // Versão
    Text version = CreateText(VERSION, 0x07, CreateVector2D(0, -1), consoleSize, 0, BOTTOM);

    // Botão de play
    Button playButton = CreateButton("Start", 0x0C, CreateVector2D(0, 0), consoleSize, UI_START, 0, CENTER);

    // Botão de informações
    Button infoButton = CreateButton("Info", 0x07, CreateVector2D(0, 2), consoleSize, UI_INFO, 0, CENTER);

    // Botão de saída
    Button quitButton = CreateButton("Quit", 0x07, CreateVector2D(0, 4), consoleSize, UI_QUIT, 0, CENTER);

    // Menu principal
    Interface mainMenu = {

        .texts = {mainMenuTitle, version},
        .buttons = {playButton, infoButton, quitButton},
        .selectedButton = 0,
        .update = 1};

    return  mainMenu;
}

Interface BuildInfoInterface(Vector2D consoleSize)
{
    // Título
    Text infoMenuTitle = CreateText(" ______  __    __  ________   ______  \n|      \\|  \\  |  \\|        \\ /   "
                                    "   \\ \n \\$$$$$$| $$\\ | $$| $$$$$$$$|  $$$$$$\\\n  | $$  | $$$\\| $$| $$__ "
                                    "   | $$  | $$\n  | $$  | $$$$\\ $$| $$  \\   | $$  | $$\n  | $$  | $$\\$$ $$|"
                                    " $$$$$   | $$  | $$\n _| $$_ | $$ \\$$$$| $$      | $$__/ $$\n|   $$ \\| $$"
                                    "  \\$$$| $$       \\$$    $$\n \\$$$$$$ \\$$   \\$$ \\$$        \\$$$$$$ ",
                                    0x0A,
                                    CreateVector2D(0, 4),
                                    consoleSize,
                                    0,
                                    TOP);

    // Informações da data
    Text creationDateInfo = CreateText("Adaptation of my first game that was created in 2019-03-19",
                                       0x07,
                                       CreateVector2D(0, 0),
                                       consoleSize,
                                       0,
                                       CENTER);

    // Link do github
    Text githubInfo = CreateText("Written by Eric (ErFer7): https://github.com/ErFer7/Survive",
                                 0x07,
                                 CreateVector2D(0, 1),
                                 consoleSize,
                                 0,
                                 CENTER);

    // Link do github
    Text controlInfo = CreateText("Use the arrows to move and X to run",
                                  0x07,
                                  CreateVector2D(0, 3),
                                  consoleSize,
                                  0,
                                  CENTER);

    // Botão de retorno
    Button returnButton = CreateButton("Back", 0x0C, CreateVector2D(0, 5), consoleSize, UI_RETURN, 0, CENTER);

    // Menu de informações
    Interface infoMenu = {

        .texts = {infoMenuTitle, creationDateInfo, githubInfo, controlInfo},
        .buttons = {returnButton},
        .selectedButton = 0,
        .update = 1};

    return infoMenu;
}

Interface BuildStartInterface(Vector2D consoleSize)
{
    Text startMenuTitle = CreateText("  ______  ________   ______   _______  ________\n /      \\|        \\ /     "
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

    Text message = CreateText("Choose your game mode and world size",
                              0x07,
                              CreateVector2D(0, 0),
                              consoleSize,
                              1,
                              CENTER);

    Text warning = CreateText("Large worlds can use a lot of memory!",
                              0x0E,
                              CreateVector2D(0, 1),
                              consoleSize,
                              1,
                              CENTER);

    Button world128 = CreateButton("Small  ", 0x07, CreateVector2D(0, 3), consoleSize, UI_START_SMALL, 0, CENTER);
    Button world512 = CreateButton("Regular", 0x0C, CreateVector2D(0, 4), consoleSize, UI_START_REGULAR, 0, CENTER);
    Button world2048 = CreateButton("Large  ", 0x07, CreateVector2D(0, 5), consoleSize, UI_START_LARGE, 0, CENTER);
    Button world8192 = CreateButton("MEGA   ", 0x07, CreateVector2D(0, 6), consoleSize, UI_START_MEGA, 0, CENTER);
    Button classic = CreateButton("Classic", 0x07, CreateVector2D(0, 7), consoleSize, UI_START_CLASSIC, 0, CENTER);

    Button returnButton = CreateButton("Back", 0x07, CreateVector2D(0, 9), consoleSize, UI_RETURN, 0, CENTER);

    Interface startMenu = {

        .texts = {startMenuTitle, message, warning},
        .buttons = {world128, world512, world2048,world8192, classic, returnButton},
        .selectedButton = 1,
        .update = 1};

    return startMenu;
}

Interface BuildGameplayInterface(Vector2D consoleSize)
{
    // FPS
    Text fpsCounter = CreateText("FPS: 0000.000", 0x07, CreateVector2D(0, 0), consoleSize, 1, BOTTOM_LEFT);

    // Behaviour updates per second
    Text bupsCounter = CreateText("BLT: 0000.000 ms", 0x07, CreateVector2D(15, 0), consoleSize, 1, BOTTOM_LEFT);

    // Ticks
    Text tickCounter = CreateText("PLT: 0000.000 ms", 0x07, CreateVector2D(33, 0), consoleSize, 1, BOTTOM_LEFT);

    // Contador da pontuação
    Text scoreCounter = CreateText("Score: 0000000000", 0x07, CreateVector2D(-24, 0), consoleSize, 1, BOTTOM_RIGHT);

    // Interface de gameplay
    Interface gameplay = {

        .texts = {fpsCounter, bupsCounter, tickCounter, scoreCounter},
        .update = 0};

    return gameplay;
}

Interface BuildPauseInterface(Vector2D consoleSize)
{
    // Título
    Text pauseTitle = CreateText("$$$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$$\\ $$$$$$$\\  \n$$  __"
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

    // Botão de continuar
    Button resumeButton = CreateButton("Resume", 0x0C, CreateVector2D(0, 0), consoleSize, UI_RESUME, 0, CENTER);

    // Botão de reiniciar
    Button restartButton = CreateButton("Restart", 0x07, CreateVector2D(0, 2), consoleSize, UI_RESTART, 0, CENTER);

    // Botão de retornar para o menu
    Button menuButton = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, 0, CENTER);

    // Interface de pausa
    Interface pause = {

        .texts = {pauseTitle},
        .buttons = {resumeButton, restartButton, menuButton},
        .selectedButton = 0,
        .update = 1};

    return pause;
}

Interface BuildGameoverInterface(Vector2D consoleSize)
{
    // Título
    Text gameoverTitle = CreateText(" $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\  $$$$$$\\  $$\\    $$\\ $$$$$$"
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

    // Pontuação final
    Text finalScore = CreateText("Score: 0000000000", 0x07, CreateVector2D(0, 0), consoleSize, 0, CENTER);

    // Botão de reiniciar
    Button restartButton = CreateButton("Restart", 0x0C, CreateVector2D(0, 2), consoleSize, UI_RESTART, 0, CENTER);

    // Botão de retornar para o menu
    Button menuButton = CreateButton("Menu", 0x07, CreateVector2D(0, 4), consoleSize, UI_RETURN, 0, CENTER);

    // Interface de fim de jogo
    Interface gameover = {

        .texts = {gameoverTitle, finalScore},
        .buttons = {restartButton, menuButton},
        .selectedButton = 0,
        .update = 1};

    return gameover;
}

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
