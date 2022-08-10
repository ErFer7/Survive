#include "../include/interface.h"

#include "../include/core.h"
#include "../include/utilities.h"
#include "../include/graphics.h"

Interface mainMenu;
Interface infoMenu;
Interface gameplay;
Interface pause;
Interface gameover;
int interfaceKeyLock;

void CalculateAlignedPosition(int *x, int *y, int sizeX, int sizeY, enum Alignment alignment)
{
    /* Calcula a posição com base no alinhamento e retorna.
     */

    switch (alignment)
    {
    case TOP:

        *x += (consoleWidth - sizeX) / 2;
        break;
    case TOP_RIGHT:

        *x += (consoleWidth + sizeX / 2) - 1;
        break;
    case LEFT:

        *y += (consoleHeight - sizeY) / 2;
        break;
    case CENTER:

        *x += (consoleWidth - sizeX) / 2;
        *y += (consoleHeight - sizeY) / 2;
        break;
    case RIGHT:

        *x += (consoleWidth + sizeX / 2) - 1;
        *y += (consoleHeight - sizeY) / 2;
        break;
    case BOTTOM_LEFT:

        *y += (consoleHeight + sizeY / 2) - 1;
        break;
    case BOTTOM:

        *x += (consoleWidth - sizeX) / 2;
        *y += (consoleHeight + sizeY / 2) - 1;
        break;
    case BOTTOM_RIGHT:

        *x += (consoleWidth + sizeX / 2) - 1;
        *y += (consoleHeight + sizeY / 2) - 1;
        break;
    default:
        break;
    }
}

void InterfaceBehaviour(Interface *interfaceIn)
{
    /* Define o comportamento de uma interface.
     */
    if (!interfaceKeyLock)
    {
        if (GetKeyState(VK_RETURN) & 0x8000) // Enter
        {
            if (interfaceIn->selectedButton < 0)
            {
                interfaceIn->selectedButton = 0;
            }

            // Adiciona o evento do botão na lista de eventos caso o botão seja válido
            if (interfaceIn->buttons[interfaceIn->selectedButton].event != IDLE)
            {
                LockEvent();
                SetGameEvent(interfaceIn->buttons[interfaceIn->selectedButton].event, 0);
                UnlockEvent();
                interfaceIn->update = 1;
            }

            interfaceKeyLock = 1;
        }
        else if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
        {
            // Seleciona o botão superior caso possível
            if (interfaceIn->selectedButton > 0)
            {
                interfaceIn->selectedButton--;
                interfaceIn->buttons[interfaceIn->selectedButton].color = 0x0C;
                interfaceIn->buttons[interfaceIn->selectedButton + 1].color = 0x07;
                interfaceIn->buttons[interfaceIn->selectedButton].update = 1;
                interfaceIn->buttons[interfaceIn->selectedButton + 1].update = 1;
            }

            interfaceKeyLock = 1;
        }
        else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
        {
            // Seleciona o botão inferior caso possível
            if (interfaceIn->selectedButton < MAX_BUTTONS - 1 &&
                interfaceIn->buttons[interfaceIn->selectedButton + 1].event != IDLE)
            {
                interfaceIn->selectedButton++;
                interfaceIn->buttons[interfaceIn->selectedButton].color = 0x0C;
                interfaceIn->buttons[interfaceIn->selectedButton - 1].color = 0x07;
                interfaceIn->buttons[interfaceIn->selectedButton].update = 1;
                interfaceIn->buttons[interfaceIn->selectedButton - 1].update = 1;
            }

            interfaceKeyLock = 1;
        }
        else if (GetKeyState(VK_ESCAPE) & 0x8000) // Esc
        {
            // Adiciona o evento correspondente ao estado na lista de eventos
            LockEvent();
            switch (state)
            {
            case MAIN_MENU:

                SetGameEvent(UI_QUIT, 0); // Evento de saída
                break;
            case INFO_MENU:

                SetGameEvent(UI_RETURN, 0); // Evento de retorno ao menu
                interfaceIn->update = 1;
                break;
            case GAMEPLAY:

                SetGameEvent(UI_PAUSE, 0); // Evento de pausa
                break;
            case PAUSE:

                SetGameEvent(UI_RESUME, 0); // Evento de continuar o jogo
                interfaceIn->update = 1;
                break;
            case GAMEOVER:

                SetGameEvent(UI_RETURN, 0); // Evento de retornar ao menu
                interfaceIn->update = 1;
                break;
            default:
                break;
            }
            UnlockEvent();

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

void UpdateInterfaces()
{
    /* Atualiza todas as interfaces com base no estado.
     */

    switch (state)
    {
    case MAIN_MENU:

        RenderInterface(&mainMenu);
        InterfaceBehaviour(&mainMenu);
        break;
    case INFO_MENU:

        RenderInterface(&infoMenu);
        InterfaceBehaviour(&infoMenu);
        break;
    case GAMEPLAY:

        RenderInterface(&gameplay);
        InterfaceBehaviour(&gameplay);
        break;
    case PAUSE:

        RenderInterface(&pause);
        InterfaceBehaviour(&pause);
        break;
    case GAMEOVER:

        RenderInterface(&gameover);
        InterfaceBehaviour(&gameover);
        break;
    default:
        break;
    }
}

void RenderInterface(Interface *interfaceIn)
{
    /* Renderiza a interface.
     */

    // Limpa a tela caso toda a interface deva ser atualizada
    if (interfaceIn->update)
    {
        ClearOutput();
    }

    // Renderiza cada texto
    for (int i = 0; i < MAX_TEXTS; i++)
    {
        if (interfaceIn->update || interfaceIn->texts[i].update)
        {
            PrintStringOnPosition(interfaceIn->texts[i].content,
                                  interfaceIn->texts[i].color,
                                  interfaceIn->texts[i].position[0],
                                  interfaceIn->texts[i].position[1]);

            interfaceIn->texts[i].update = 0;
        }
    }

    // Renderiza cada botão
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        if (interfaceIn->update || interfaceIn->buttons[i].update)
        {
            PrintStringOnPosition(interfaceIn->buttons[i].content,
                                  interfaceIn->buttons[i].color,
                                  interfaceIn->buttons[i].position[0],
                                  interfaceIn->buttons[i].position[1]);

            interfaceIn->buttons[i].update = 0;
        }
    }

    // Move o cursor para o canto da tela
    SetCursorPosition(consoleWidth - 1, consoleHeight - 1);

    interfaceIn->update = 0; // Reseta o estado de atualização da interface

    WriteOutput();
}

void InitInterface()
{
    // Inicializa o menu principal

    // Título
    Text mainMenuTitle = {

        .content = "  ______   __    __  _______   __     __  ______  __     __  ________  \n"
                   " /      \\ |  \\  |  \\|       \\ |  \\   |  \\|      \\|  \\   |  \\|   "
                   "     \\\n|  $$$$$$\\| $$  | $$| $$$$$$$\\| $$   | $$ \\$$$$$$| $$   | $$|"
                   " $$$$$$$$\n| $$___\\$$| $$  | $$| $$__| $$| $$   | $$  | $$  | $$   | $$|"
                   " $$__    \n \\$$    \\ | $$  | $$| $$    $$ \\$$\\ /  $$  | $$   \\$$\\ /"
                   "  $$| $$  \\   \n _\\$$$$$$\\| $$  | $$| $$$$$$$\\  \\$$\\  $$   | $$    "
                   "\\$$\\  $$ | $$$$$   \n|  \\__| $$| $$__/ $$| $$  | $$   \\$$ $$   _| $$_"
                   "    \\$$ $$  | $$_____ \n \\$$    $$ \\$$    $$| $$  | $$    \\$$$   |   "
                   "$$ \\    \\$$$   | $$     \\\n  \\$$$$$$   \\$$$$$$  \\$$   \\$$     \\$ "
                   "    \\$$$$$$     \\$     \\$$$$$$$$",
        .color = 0x0C,
        .position = {0, 4},
        .update = 0};

    CalculateAlignedPosition(&mainMenuTitle.position[0],
                             &mainMenuTitle.position[1],
                             70,
                             9,
                             TOP);

    // Versão
    Text version = {

        .content = VERSION,
        .color = 0x07,
        .position = {0, -1},
        .update = 0};

    CalculateAlignedPosition(&version.position[0],
                             &version.position[1],
                             4,
                             1,
                             BOTTOM);

    // Botão de play
    Button playButton = {

        .content = "Play",
        .color = 0x07,
        .position = {0, 0},
        .event = UI_PLAY,
        .update = 0};

    CalculateAlignedPosition(&playButton.position[0],
                             &playButton.position[1],
                             4,
                             1,
                             CENTER);

    // Botão de informações
    Button infoButton = {

        .content = "Info",
        .color = 0x07,
        .position = {0, 2},
        .event = UI_INFO,
        .update = 0};

    CalculateAlignedPosition(&infoButton.position[0],
                             &infoButton.position[1],
                             4,
                             1,
                             CENTER);

    // Botão de saída
    Button quitButton = {

        .content = "Quit",
        .color = 0x07,
        .position = {0, 4},
        .event = UI_QUIT,
        .update = 0};

    CalculateAlignedPosition(&quitButton.position[0],
                             &quitButton.position[1],
                             4,
                             1,
                             CENTER);

    // Menu principal
    Interface mainMenuInit = {

        .texts = {mainMenuTitle, version},
        .buttons = {playButton, infoButton, quitButton},
        .selectedButton = -1,
        .update = 1};

    mainMenu = mainMenuInit;

    // Inicializa o menu de informações

    // Título
    Text infoMenuTitle = {

        .content = " ______  __    __  ________   ______  \n|      \\|  \\  |  \\|        \\ /   "
                   "   \\ \n \\$$$$$$| $$\\ | $$| $$$$$$$$|  $$$$$$\\\n  | $$  | $$$\\| $$| $$__ "
                   "   | $$  | $$\n  | $$  | $$$$\\ $$| $$  \\   | $$  | $$\n  | $$  | $$\\$$ $$|"
                   " $$$$$   | $$  | $$\n _| $$_ | $$ \\$$$$| $$      | $$__/ $$\n|   $$ \\| $$"
                   "  \\$$$| $$       \\$$    $$\n \\$$$$$$ \\$$   \\$$ \\$$        \\$$$$$$ ",
        .color = 0x0A,
        .position = {0, 4},
        .update = 0};

    CalculateAlignedPosition(&infoMenuTitle.position[0],
                             &infoMenuTitle.position[1],
                             40,
                             9,
                             TOP);

    // Informações da data
    Text creationDateInfo = {

        .content = "Adaptation of my first game that was created in 2019-03-19",
        .color = 0x07,
        .position = {0, 0},
        .update = 0};

    CalculateAlignedPosition(&creationDateInfo.position[0],
                             &creationDateInfo.position[1],
                             58,
                             1,
                             CENTER);

    // Link do github
    Text githubInfo = {

        .content = "Written by Eric (ErFer7): https://github.com/ErFer7/Survive",
        .color = 0x07,
        .position = {0, 1},
        .update = 0};

    CalculateAlignedPosition(&githubInfo.position[0],
                             &githubInfo.position[1],
                             59,
                             1,
                             CENTER);

    // Link do github
    Text controlInfo = {

        .content = "Use the arrows to move and X to run",
        .color = 0x07,
        .position = {0, 3},
        .update = 0};

    CalculateAlignedPosition(&controlInfo.position[0],
                             &controlInfo.position[1],
                             35,
                             1,
                             CENTER);

    // Botão de retorno
    Button returnButton = {

        .content = "Back",
        .color = 0x07,
        .position = {0, 5},
        .event = UI_RETURN,
        .update = 0};

    CalculateAlignedPosition(&returnButton.position[0],
                             &returnButton.position[1],
                             4,
                             1,
                             CENTER);

    // Menu de informações
    Interface infoMenuInit = {

        .texts = {infoMenuTitle, creationDateInfo, githubInfo, controlInfo},
        .buttons = {returnButton},
        .selectedButton = -1,
        .update = 1};

    infoMenu = infoMenuInit;

    // Inicializa interface do gameplay

    // FPS
    Text fpsCounter = {

        .content = "FPS: 0000.000",
        .color = 0x07,
        .position = {0, 0},
        .update = 1};

    CalculateAlignedPosition(&fpsCounter.position[0],
                             &fpsCounter.position[1],
                             13,
                             1,
                             BOTTOM_LEFT);

    // Behaviour updates per second
    Text bupsCounter = {

        .content = "BLT: 0000.000 ms",
        .color = 0x07,
        .position = {15, 0},
        .update = 1};

    CalculateAlignedPosition(&bupsCounter.position[0],
                             &bupsCounter.position[1],
                             16,
                             1,
                             BOTTOM_LEFT);

    // Ticks
    Text tickCounter = {

        .content = "PLT: 0000.000 ms",
        .color = 0x07,
        .position = {33, 0},
        .update = 1};

    CalculateAlignedPosition(&tickCounter.position[0],
                             &tickCounter.position[1],
                             16,
                             1,
                             BOTTOM_LEFT);

    // Contador da pontuação
    Text scoreCounter = {

        .content = "Score: 0000000000",
        .color = 0x07,
        .position = {-24, 0},
        .update = 1};

    CalculateAlignedPosition(&scoreCounter.position[0],
                             &scoreCounter.position[1],
                             17,
                             1,
                             BOTTOM_RIGHT);

    // Interface de gameplay
    Interface gameplayInit = {

        .texts = {fpsCounter, bupsCounter, tickCounter, scoreCounter},
        .update = 0};

    gameplay = gameplayInit;

    // Inicializa a tela de pausa

    // Título
    Text pauseTitle = {

        .content = "$$$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$$$$$$$\\ $$$$$$$\\  \n$$  __"
                   "$$\\ $$  __$$\\ $$ |  $$ |$$  __$$\\ $$  _____|$$  __$$\\ \n$$ |  $$ |$$ / "
                   " $$ |$$ |  $$ |$$ /  \\__|$$ |      $$ |  $$ |\n$$$$$$$  |$$$$$$$$ |$$ |  $"
                   "$ |\\$$$$$$\\  $$$$$\\    $$ |  $$ |\n$$  ____/ $$  __$$ |$$ |  $$ | \\____"
                   "$$\\ $$  __|   $$ |  $$ |\n$$ |      $$ |  $$ |$$ |  $$ |$$\\   $$ |$$ |   "
                   "   $$ |  $$ |\n$$ |      $$ |  $$ |\\$$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$"
                   "  |\n\\__|      \\__|  \\__| \\______/  \\______/ \\________|\\_______/ ",
        .color = 0x07,
        .position = {0, 4},
        .update = 0};

    CalculateAlignedPosition(&pauseTitle.position[0],
                             &pauseTitle.position[1],
                             60,
                             9,
                             TOP);

    // Botão de continuar
    Button resumeButton = {

        .content = "Resume",
        .color = 0x07,
        .position = {0, 0},
        .event = UI_RESUME,
        .update = 0};

    CalculateAlignedPosition(&resumeButton.position[0],
                             &resumeButton.position[1],
                             6,
                             1,
                             CENTER);

    // Botão de reiniciar
    Button restartButton = {

        .content = "Restart",
        .color = 0x07,
        .position = {0, 2},
        .event = UI_RESTART,
        .update = 0};

    CalculateAlignedPosition(&restartButton.position[0],
                             &restartButton.position[1],
                             7,
                             1,
                             CENTER);

    // Botão de retornar para o menu
    Button menuButton = {

        .content = "Menu",
        .color = 0x07,
        .position = {0, 4},
        .event = UI_RETURN,
        .update = 0};

    CalculateAlignedPosition(&menuButton.position[0],
                             &menuButton.position[1],
                             4,
                             1,
                             CENTER);

    // Interface de pausa
    Interface pauseInit = {

        .texts = {pauseTitle},
        .buttons = {resumeButton, restartButton, menuButton},
        .selectedButton = -1,
        .update = 1};

    pause = pauseInit;

    // Inicializa a interface de fim de jogo

    // Título
    Text gameoverTitle = {

        .content = " $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\  $$$$$$\\  $$\\    $$\\ $$$$$$"
                   "$$\\ $$$$$$$\\  \n$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|$$  __$$\\ $$ "
                   "|   $$ |$$  _____|$$  __$$\\ \n$$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      $$"
                   " /  $$ |$$ |   $$ |$$ |      $$ |  $$ |\n$$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$"
                   "$$$$\\    $$ |  $$ |\\$$\\  $$  |$$$$$\\    $$$$$$$  |\n$$ |\\_$$ |$$  __$$ |$"
                   "$ \\$$$  $$ |$$  __|   $$ |  $$ | \\$$\\$$  / $$  __|   $$  __$$< \n$$ |  $$ |"
                   "$$ |  $$ |$$ |\\$  /$$ |$$ |      $$ |  $$ |  \\$$$  /  $$ |      $$ |  $$ |\n"
                   "\\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\  $$$$$$  |   \\$  /   $$$$$$$$\\"
                   " $$ |  $$ |\n \\______/ \\__|  \\__|\\__|     \\__|\\________| \\______/     "
                   "\\_/    \\________|\\__|  \\__|",
        .color = 0x0C,
        .position = {0, 4},
        .update = 0};

    CalculateAlignedPosition(&gameoverTitle.position[0],
                             &gameoverTitle.position[1],
                             84,
                             9,
                             TOP);

    // Pontuação final
    Text finalScore = {

        .content = "Score: 0000000000",
        .color = 0x07,
        .position = {0, 0},
        .update = 0};

    CalculateAlignedPosition(&finalScore.position[0],
                             &finalScore.position[1],
                             17,
                             1,
                             CENTER);

    // Interface de fim de jogo
    Interface gameoverInit = {

        .texts = {gameoverTitle, finalScore},
        .buttons = {restartButton, menuButton},
        .selectedButton = -1,
        .update = 1};

    gameover = gameoverInit;

    interfaceKeyLock = 0;
}
