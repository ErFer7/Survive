#include "../include/interface.h"

#include <stdio.h>
#include <pthread.h>

#include "../include/vector2D.h"
#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"

Interface mainMenu;
Interface startMenu;
Interface infoMenu;
Interface gameplay;
Interface pause;
Interface gameover;
int interfaceKeyLock;

void CalculateAlignedPosition(char *string, int *x, int *y, enum Alignment alignment)
{
    /* Calcula a posição com base no alinhamento e retorna.
     */

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

        *x += (consoleWidth - width) / 2;
        break;
    case TOP_RIGHT:

        *x += (consoleWidth + width / 2) - 1;
        break;
    case LEFT:

        *y += (consoleHeight - height) / 2;
        break;
    case CENTER:

        *x += (consoleWidth - width) / 2;
        *y += (consoleHeight - height) / 2;
        break;
    case RIGHT:

        *x += (consoleWidth + width / 2) - 1;
        *y += (consoleHeight - height) / 2;
        break;
    case BOTTOM_LEFT:

        *y += (consoleHeight + height / 2) - 1;
        break;
    case BOTTOM:

        *x += (consoleWidth - width) / 2;
        *y += (consoleHeight + height / 2) - 1;
        break;
    case BOTTOM_RIGHT:

        *x += (consoleWidth + width / 2) - 1;
        *y += (consoleHeight + height / 2) - 1;
        break;
    default:
        break;
    }
}

Text CreateText(char content[MAX_TEXT_STRLEN],
                unsigned short color,
                int x,
                int y,
                int update,
                enum Alignment alignment)
{
    Text text = {
        .content = "",
        .color = color,
        .position = {x, y},
        .update = update};

    memcpy(text.content, content, sizeof(char) * MAX_TEXT_STRLEN);

    CalculateAlignedPosition(text.content,
                             &text.position[0],
                             &text.position[1],
                             alignment);

    return text;
}

Button CreateButton(char content[MAX_BUTTON_STRLEN],
                    unsigned short color,
                    int x,
                    int y,
                    enum Event event,
                    int update,
                    enum Alignment alignment)
{
    Button button = {
        .content = "",
        .color = color,
        .position = {x, y},
        .event = event,
        .update = update};

    memcpy(button.content, content, sizeof(char) * MAX_BUTTON_STRLEN);

    CalculateAlignedPosition(button.content,
                             &button.position[0],
                             &button.position[1],
                             alignment);

    return button;
}

void InterfaceBehaviour(EventStateContext *eventStateContextPtr, Interface *interface_)
{
    /* Define o comportamento de uma interface.
     */
    if (!interfaceKeyLock)
    {
        if (GetKeyState(VK_RETURN) & 0x8000) // Enter
        {
            // Adiciona o evento do botão na lista de eventos caso o botão seja válido
            if (interface_->buttons[interface_->selectedButton].event != IDLE)
            {
                pthread_mutex_lock(&eventStateContextPtr->eventMutex);
                if (eventStateContextPtr->event == IDLE)
                {
                    eventStateContextPtr->event = interface_->buttons[interface_->selectedButton].event;
                }
                pthread_mutex_unlock(&eventStateContextPtr->eventMutex);
                interface_->update = 1;
            }

            interfaceKeyLock = 1;
        }
        else if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
        {
            // Seleciona o botão superior caso possível
            if (interface_->selectedButton > 0)
            {
                interface_->selectedButton--;
                interface_->buttons[interface_->selectedButton].color = 0x0C;
                interface_->buttons[interface_->selectedButton + 1].color = 0x07;
                interface_->buttons[interface_->selectedButton].update = 1;
                interface_->buttons[interface_->selectedButton + 1].update = 1;
            }

            interfaceKeyLock = 1;
        }
        else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
        {
            // Seleciona o botão inferior caso possível
            if (interface_->selectedButton < MAX_BUTTONS - 1 &&
                interface_->buttons[interface_->selectedButton + 1].event != IDLE)
            {
                interface_->selectedButton++;
                interface_->buttons[interface_->selectedButton].color = 0x0C;
                interface_->buttons[interface_->selectedButton - 1].color = 0x07;
                interface_->buttons[interface_->selectedButton].update = 1;
                interface_->buttons[interface_->selectedButton - 1].update = 1;
            }

            interfaceKeyLock = 1;
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
                    interface_->update = 1;
                    break;
                case START_MENU:

                    eventStateContextPtr->event = UI_RETURN;
                    interface_->update = 1;
                    break;
                case GAMEPLAY:

                    eventStateContextPtr->event = UI_PAUSE;
                    break;
                case PAUSE:

                    eventStateContextPtr->event = UI_RESUME;
                    interface_->update = 1;
                    break;
                case GAMEOVER:

                    eventStateContextPtr->event = UI_RETURN;
                    interface_->update = 1;
                    break;
                default:
                    break;
                }
            }
            pthread_mutex_unlock(&eventStateContextPtr->eventMutex);

            interfaceKeyLock = 1;
        }
    }

    if (interfaceKeyLock && !(GetKeyState(VK_RETURN) & 0x8000 ||
                              GetKeyState(VK_ESCAPE) & 0x8000 ||
                              GetKeyState(VK_UP) & 0x8000 ||
                              GetKeyState(VK_DOWN) & 0x8000))
    {
        interfaceKeyLock = 0;
    }
}

void UpdateInterfaces(EventStateContext *eventStateContextPtr)
{
    /* Atualiza todas as interfaces com base no estado.
     */

    switch (eventStateContextPtr->state)
    {
    case MAIN_MENU:

        RenderInterface(&mainMenu);
        InterfaceBehaviour(eventStateContextPtr, &mainMenu);
        break;
    case INFO_MENU:

        RenderInterface(&infoMenu);
        InterfaceBehaviour(eventStateContextPtr, &infoMenu);
        break;
    case START_MENU:
        RenderInterface(&startMenu);
        InterfaceBehaviour(eventStateContextPtr, &startMenu);
        break;
    case GAMEPLAY:

        RenderInterface(&gameplay);
        InterfaceBehaviour(eventStateContextPtr, &gameplay);
        break;
    case PAUSE:

        RenderInterface(&pause);
        InterfaceBehaviour(eventStateContextPtr, &pause);
        break;
    case GAMEOVER:

        RenderInterface(&gameover);
        InterfaceBehaviour(eventStateContextPtr, &gameover);
        break;
    default:
        break;
    }
}

void RenderInterface(Interface *interface_)
{
    /* Renderiza a interface.
     */

    // Limpa a tela caso toda a interface deva ser atualizada
    if (interface_->update)
    {
        ClearOutput();
    }

    // Renderiza cada texto
    for (int i = 0; i < MAX_TEXTS; i++)
    {
        if (interface_->update || interface_->texts[i].update)
        {
            PrintStringOnPosition(interface_->texts[i].content,
                                  interface_->texts[i].color,
                                  interface_->texts[i].position[0],
                                  interface_->texts[i].position[1]);

            interface_->texts[i].update = 0;
        }
    }

    // Renderiza cada botão
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        if (interface_->update || interface_->buttons[i].update)
        {
            PrintStringOnPosition(interface_->buttons[i].content,
                                  interface_->buttons[i].color,
                                  interface_->buttons[i].position[0],
                                  interface_->buttons[i].position[1]);

            interface_->buttons[i].update = 0;
        }
    }

    // Move o cursor para o canto da tela
    SetCursorPosition(CreateVector2D(consoleWidth - 1, consoleHeight - 1));

    interface_->update = 0; // Reseta o estado de atualização da interface

    WriteOutput();
}

void BuildMainMenuInterface()
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
                                    0,
                                    4,
                                    0,
                                    TOP);

    // Versão
    Text version = CreateText(VERSION, 0x07, 0, -1, 0, BOTTOM);

    // Botão de play
    Button playButton = CreateButton("Start", 0x0C, 0, 0, UI_START, 0, CENTER);

    // Botão de informações
    Button infoButton = CreateButton("Info", 0x07, 0, 2, UI_INFO, 0, CENTER);

    // Botão de saída
    Button quitButton = CreateButton("Quit", 0x07, 0, 4, UI_QUIT, 0, CENTER);

    // Menu principal
    Interface mainMenuInit = {

        .texts = {mainMenuTitle, version},
        .buttons = {playButton, infoButton, quitButton},
        .selectedButton = 0,
        .update = 1};

    mainMenu = mainMenuInit;
}

void BuildInfoInterface()
{
    // Título
    Text infoMenuTitle = CreateText(" ______  __    __  ________   ______  \n|      \\|  \\  |  \\|        \\ /   "
                                    "   \\ \n \\$$$$$$| $$\\ | $$| $$$$$$$$|  $$$$$$\\\n  | $$  | $$$\\| $$| $$__ "
                                    "   | $$  | $$\n  | $$  | $$$$\\ $$| $$  \\   | $$  | $$\n  | $$  | $$\\$$ $$|"
                                    " $$$$$   | $$  | $$\n _| $$_ | $$ \\$$$$| $$      | $$__/ $$\n|   $$ \\| $$"
                                    "  \\$$$| $$       \\$$    $$\n \\$$$$$$ \\$$   \\$$ \\$$        \\$$$$$$ ",
                                    0x0A,
                                    0,
                                    4,
                                    0,
                                    TOP);

    // Informações da data
    Text creationDateInfo = CreateText("Adaptation of my first game that was created in 2019-03-19",
                                       0x07,
                                       0,
                                       0,
                                       0,
                                       CENTER);

    // Link do github
    Text githubInfo = CreateText("Written by Eric (ErFer7): https://github.com/ErFer7/Survive",
                                 0x07,
                                 0,
                                 1,
                                 0,
                                 CENTER);

    // Link do github
    Text controlInfo = CreateText("Use the arrows to move and X to run",
                                  0x07,
                                  0,
                                  3,
                                  0,
                                  CENTER);

    // Botão de retorno
    Button returnButton = CreateButton("Back", 0x0C, 0, 5, UI_RETURN, 0, CENTER);

    // Menu de informações
    Interface infoMenuInit = {

        .texts = {infoMenuTitle, creationDateInfo, githubInfo, controlInfo},
        .buttons = {returnButton},
        .selectedButton = 0,
        .update = 1};

    infoMenu = infoMenuInit;
}

void BuildStartInterface()
{
    Text startMenuTitle = CreateText("  ______  ________   ______   _______  ________\n /      \\|        \\ /     "
                                     " \\ |       \\|        \\\n|  $$$$$$\\\\$$$$$$$$|  $$$$$$\\| $$$$$$$\\\\$$$$$"
                                     "$$$\n| $$___\\$$  | $$   | $$__| $$| $$__| $$  | $$\n \\$$    \\   | $$   | $"
                                     "$    $$| $$    $$  | $$\n _\\$$$$$$\\  | $$   | $$$$$$$$| $$$$$$$\\  | $$\n| "
                                     " \\__| $$  | $$   | $$  | $$| $$  | $$  | $$\n \\$$    $$  | $$   | $$  | $$|"
                                     " $$  | $$  | $$\n  \\$$$$$$    \\$$    \\$$   \\$$ \\$$   \\$$   \\$$",
                                     0x0C,
                                     0,
                                     4,
                                     0,
                                     TOP);

    Text message = CreateText("Choose your game mode and world size", 0x07, 0, 0, 1, CENTER);
    Text warning = CreateText("Large worlds can use a lot of memory!", 0x0E, 0, 1, 1, CENTER);

    Button world128 = CreateButton("Small  ", 0x07, 0, 3, UI_START_SMALL, 0, CENTER);
    Button world512 = CreateButton("Regular", 0x0C, 0, 4, UI_START_REGULAR, 0, CENTER);
    Button world2048 = CreateButton("Large  ", 0x07, 0, 5, UI_START_LARGE, 0, CENTER);
    Button world8192 = CreateButton("MEGA   ", 0x07, 0, 6, UI_START_MEGA, 0, CENTER);
    Button classic = CreateButton("Classic", 0x07, 0, 7, UI_START_CLASSIC, 0, CENTER);

    Button returnButton = CreateButton("Back", 0x07, 0, 9, UI_RETURN, 0, CENTER);

    Interface startMenuInit = {

        .texts = {startMenuTitle, message, warning},
        .buttons = {world128, world512, world2048,world8192, classic, returnButton},
        .selectedButton = 1,
        .update = 1};

    startMenu = startMenuInit;
}

void BuildGameplayInterface()
{
    // FPS
    Text fpsCounter = CreateText("FPS: 0000.000", 0x07, 0, 0, 1, BOTTOM_LEFT);

    // Behaviour updates per second
    Text bupsCounter = CreateText("BLT: 0000.000 ms", 0x07, 15, 0, 1, BOTTOM_LEFT);

    // Ticks
    Text tickCounter = CreateText("PLT: 0000.000 ms", 0x07, 33, 0, 1, BOTTOM_LEFT);

    // Contador da pontuação
    Text scoreCounter = CreateText("Score: 0000000000", 0x07, -24, 0, 1, BOTTOM_RIGHT);

    // Interface de gameplay
    Interface gameplayInit = {

        .texts = {fpsCounter, bupsCounter, tickCounter, scoreCounter},
        .update = 0};

    gameplay = gameplayInit;
}

void BuildPauseInterface()
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
                                 0,
                                 4,
                                 0,
                                 TOP);

    // Botão de continuar
    Button resumeButton = CreateButton("Resume", 0x0C, 0, 0, UI_RESUME, 0, CENTER);

    // Botão de reiniciar
    Button restartButton = CreateButton("Restart", 0x07, 0, 2, UI_RESTART, 0, CENTER);

    // Botão de retornar para o menu
    Button menuButton = CreateButton("Menu", 0x07, 0, 4, UI_RETURN, 0, CENTER);

    // Interface de pausa
    Interface pauseInit = {

        .texts = {pauseTitle},
        .buttons = {resumeButton, restartButton, menuButton},
        .selectedButton = 0,
        .update = 1};

    pause = pauseInit;
}

void BuildGameoverInterface()
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
                                    0,
                                    4,
                                    0,
                                    TOP);

    // Pontuação final
    Text finalScore = CreateText("Score: 0000000000", 0x07, 0, 0, 0, CENTER);

    // Botão de reiniciar
    Button restartButton = CreateButton("Restart", 0x0C, 0, 2, UI_RESTART, 0, CENTER);

    // Botão de retornar para o menu
    Button menuButton = CreateButton("Menu", 0x07, 0, 4, UI_RETURN, 0, CENTER);

    // Interface de fim de jogo
    Interface gameoverInit = {

        .texts = {gameoverTitle, finalScore},
        .buttons = {restartButton, menuButton},
        .selectedButton = 0,
        .update = 1};

    gameover = gameoverInit;
}

void InitInterface()
{
    BuildMainMenuInterface();
    BuildInfoInterface();
    BuildStartInterface();
    BuildGameplayInterface();
    BuildPauseInterface();
    BuildGameoverInterface();

    interfaceKeyLock = 0;
}
