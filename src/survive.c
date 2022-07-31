#include "../include/survive.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

#include "../include/entity.h"
#include "../include/renderer.h"

#define VERSION "v2.9.3"
#define PLAYER_SPEED 23.5f
#define ENEMY_SPEED 13.0f
#define SELECTION_SPEED 10.0f
#define ANIMATION_SPEED 16.0f

void GameInit(unsigned int t, uint8_t width, uint8_t height)
{
    /* Inicializa o jogo.
     */

    InitConsoleRenderer((unsigned int)width, (unsigned int)height);

    consoleWidth = width > 255 ? 255 : width;
    consoleHeight = height > 255 ? 255 : height;

    for (int i = 0; i < MAX_EVENTS; i++) // Inicializa os eventos
        events[i] = IDLE;

    // Inicializa as interfaces

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
        .position = {1, 1},
        .update = 0};

    CalculateAlignedPosition(&version.position[0],
                             &version.position[1],
                             6,
                             1,
                             TOP_LEFT);

    // Botão de play
    Button playButton = {

        .content = "Play",
        .color = 0x0C,
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
        .selectedButton = 0.0f,
        .selectionSpeed = SELECTION_SPEED,
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

    // Botão de retorno
    Button returnButton = {

        .content = "Back",
        .color = 0x0C,
        .position = {0, 3},
        .event = UI_RETURN,
        .update = 0};

    CalculateAlignedPosition(&returnButton.position[0],
                             &returnButton.position[1],
                             4,
                             1,
                             CENTER);

    // Menu de informações
    Interface infoMenuInit = {

        .texts = {infoMenuTitle, creationDateInfo, githubInfo},
        .buttons = {returnButton},
        .selectedButton = 0.0f,
        .update = 1};

    infoMenu = infoMenuInit;

    // Inicializa interface do gameplay

    // FPS
    Text fpsCounter = {

        .content = "000.000 fps   ",
        .color = 0x07,
        .position = {0, 0},
        .update = 1};

    CalculateAlignedPosition(&fpsCounter.position[0],
                             &fpsCounter.position[1],
                             11,
                             1,
                             BOTTOM_LEFT);

    // Contador da pontuação
    Text scoreCounter = {

        .content = "score: 00000",
        .color = 0x07,
        .position = {-18, 0},
        .update = 1};

    CalculateAlignedPosition(&scoreCounter.position[0],
                             &scoreCounter.position[1],
                             12,
                             1,
                             BOTTOM_RIGHT);

    // Interface de gameplay
    Interface gameplayInit = {

        .texts = {fpsCounter, scoreCounter},
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
        .color = 0x0C,
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
        .selectedButton = 0.0f,
        .selectionSpeed = SELECTION_SPEED,
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

        .content = "Score: 00000",
        .color = 0x07,
        .position = {0, 0},
        .update = 0};

    CalculateAlignedPosition(&finalScore.position[0],
                             &finalScore.position[1],
                             12,
                             1,
                             CENTER);

    // Interface de fim de jogo
    Interface gameoverInit = {

        .texts = {gameoverTitle, finalScore},
        .buttons = {restartButton, menuButton},
        .selectedButton = 0.0f,
        .selectionSpeed = SELECTION_SPEED,
        .update = 1};

    interfaceKeyLock = 0;

    gameover = gameoverInit;
    tick = t;

    srand((unsigned)time(NULL)); // Inicializa o RNG
}

void BuildBorders()
{
    /* Faz as bordas da tela.
     */

    for (int i = 0; i < consoleWidth; i++)
    {
        SetCharOnPosition(i, 0, 219, 7);
        SetCharOnPosition(i, consoleHeight - 2, 219, 7);
    }

    for (int i = 0; i < consoleHeight - 1; i++)
    {
        SetCharOnPosition(0, i, 219, 7);
        SetCharOnPosition(consoleWidth - 1, i, 219, 7);
    }
}

void CalculateAlignedPosition(int16_t *x,
                              int16_t *y,
                              uint8_t sizeX,
                              uint8_t sizeY,
                              enum Alignment
                                  alignment)
{
    /* Calcula a posição com base no alinhamento e retorna.
     */

    switch (alignment)
    {
    case TOP:

        *x += (int16_t)((consoleWidth - sizeX) / 2);
        break;
    case TOP_RIGHT:

        *x += (int16_t)((consoleWidth + sizeX / 2) - 1);
        break;
    case LEFT:

        *y += (int16_t)((consoleHeight - sizeY) / 2);
        break;
    case CENTER:

        *x += (int16_t)((consoleWidth - sizeX) / 2);
        *y += (int16_t)((consoleHeight - sizeY) / 2);
        break;
    case RIGHT:

        *x += (int16_t)((consoleWidth + sizeX / 2) - 1);
        *y += (int16_t)((consoleHeight - sizeY) / 2);
        break;
    case BOTTOM_LEFT:

        *y += (int16_t)((consoleHeight + sizeY / 2) - 1);
        break;
    case BOTTOM:

        *x += (int16_t)((consoleWidth - sizeX) / 2);
        *y += (int16_t)((consoleHeight + sizeY / 2) - 1);
        break;
    case BOTTOM_RIGHT:

        *x += (int16_t)((consoleWidth + sizeX / 2) - 1);
        *y += (int16_t)((consoleHeight + sizeY / 2) - 1);
        break;
    default:
        break;
    }
}

void InterfaceBehaviour(Interface *interfaceIn)
{
    /* Define o comportamento de uma interface.
     */
    if (GetKeyState(VK_RETURN) & 0x8000 && !interfaceKeyLock) // Enter
    {
        // Adiciona o evento do botão na lista de eventos caso o botão seja válido
        if (interfaceIn->buttons[(int)interfaceIn->selectedButton].event != IDLE)
        {
            events[1] = interfaceIn->buttons[(int)interfaceIn->selectedButton].event;
            interfaceIn->update = 1;
        }

        interfaceKeyLock = 1;
    }
    else if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
    {
        // Seleciona o botão superior caso possível
        if ((int)interfaceIn->selectedButton > 0)
        {
            interfaceIn->selectedButton -= interfaceIn->selectionSpeed / (float)tick;
            interfaceIn->buttons[(int)interfaceIn->selectedButton].color = 0x0C;
            interfaceIn->buttons[(int)interfaceIn->selectedButton + 1].color = 0x07;
            interfaceIn->buttons[(int)interfaceIn->selectedButton].update = 1;
            interfaceIn->buttons[(int)interfaceIn->selectedButton + 1].update = 1;
        }
    }
    else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
    {
        // Seleciona o botão inferior caso possível
        if ((int)interfaceIn->selectedButton < MAX_BUTTONS - 1 &&
            interfaceIn->buttons[(int)interfaceIn->selectedButton + 1].event != IDLE)
        {
            interfaceIn->selectedButton += interfaceIn->selectionSpeed / (float)tick;
            interfaceIn->buttons[(int)interfaceIn->selectedButton].color = 0x0C;
            interfaceIn->buttons[(int)interfaceIn->selectedButton - 1].color = 0x07;
            interfaceIn->buttons[(int)interfaceIn->selectedButton].update = 1;
            interfaceIn->buttons[(int)interfaceIn->selectedButton - 1].update = 1;
        }
    }
    else if (GetKeyState(VK_ESCAPE) & 0x8000 && !interfaceKeyLock) // Esc
    {
        // Adiciona o evento correspondente ao estado na lista de eventos
        switch (state)
        {
        case MAIN_MENU:

            events[1] = UI_QUIT; // Evento de saída
            break;
        case INFO_MENU:

            events[1] = UI_RETURN; // Evento de retorno ao menu
            interfaceIn->update = 1;
            break;
        case GAMEPLAY:

            events[1] = UI_PAUSE; // Evento de pausa
            break;
        case PAUSE:

            events[1] = UI_RESUME; // Evento de continuar o jogo
            interfaceIn->update = 1;
            break;
        case GAMEOVER:

            events[1] = UI_RETURN; // Evento de retornar ao menu
            interfaceIn->update = 1;
            break;
        default:
            break;
        }

        interfaceKeyLock = 1;
    }

    if (interfaceKeyLock && !(GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000))
        interfaceKeyLock = 0;
}

void RenderInterface(Interface *interfaceIn)
{
    /* Renderiza a interface.
     */

    // Limpa a tela caso toda a interface deva ser atualizada
    if (interfaceIn->update)
    {
        ClearOutput();
        BuildBorders();
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

void GenerateWorld()
{
    /* Gera o mundo do jogo.
     */

    score = 0;
    idCount = 0;

    // Inicializa as matrizes
    EntityMatrixInit(&entityMatrix, consoleWidth, consoleHeight - 1);

    Entity player = {idCount++,
                     {254, 254, 254, 254},
                     0.0f,
                     ANIMATION_SPEED,
                     0,
                     0x0F,
                     {0.0f, 0.0f},
                     {(float)(consoleWidth / 2), (float)(consoleHeight / 2)},
                     PLAYER_SPEED,
                     PLAYER};

    float coinPositionX = 0.0f;
    float coinPositionY = 0.0f;

    // Não é a maneira ideal de evitar a sobreposição da moeda no jogador
    do
    {
        coinPositionX = Randomf(1, consoleWidth - 2);
        coinPositionY = Randomf(1, consoleHeight - 3);
    } while (coinPositionX == player.position[0] && coinPositionY == player.position[1]);

    Entity coin = {idCount++,
                   {45, 92, 124, 47},
                   0.0f,
                   ANIMATION_SPEED,
                   1,
                   0x0E,
                   {0.0f, 0.0f},
                   {coinPositionX, coinPositionY},
                   0.0f,
                   COIN};

    // Insere o jogador e moeda na matriz

    InsertEntityOnMatrix(&entityMatrix,
                         player,
                         (uint8_t)player.position[0],
                         (uint8_t)player.position[1]);

    InsertEntityOnMatrix(&entityMatrix,
                         coin,
                         (uint8_t)coin.position[0],
                         (uint8_t)coin.position[1]);

    // Constroi as paredes de cima e de baixo
    for (int i = 0; i < consoleWidth; i++)
    {
        Entity topWall = {idCount++,
                          {219, 223, 219, 220},
                          (float)(i % MAX_ANIM_FRAMES),
                          ANIMATION_SPEED,
                          1,
                          0x87,
                          {0.0f, 0.0f},
                          {(float)i, 0.0f},
                          0.0f,
                          WALL};

        Entity bottonWall = {idCount++,
                             {219, 223, 219, 220},
                             (float)(i % MAX_ANIM_FRAMES),
                             ANIMATION_SPEED,
                             1,
                             0x87,
                             {0.0f, 0.0f},
                             {(float)i, (float)(consoleHeight - 2)},
                             0.0f,
                             WALL};

        InsertEntityOnMatrix(&entityMatrix,
                             topWall,
                             (uint8_t)topWall.position[0],
                             (uint8_t)topWall.position[1]);

        InsertEntityOnMatrix(&entityMatrix,
                             bottonWall,
                             (uint8_t)bottonWall.position[0],
                             (uint8_t)bottonWall.position[1]);
    }

    // Constroi as paredes da esquerda e direita
    for (int i = 0; i < consoleHeight - 2; i++)
    {
        Entity leftWall = {idCount++,
                           {219, 178, 219, 178},
                           (float)(i % MAX_ANIM_FRAMES),
                           ANIMATION_SPEED,
                           1,
                           0x87,
                           {0.0f, 0.0f},
                           {0.0f, (float)i},
                           0.0f,
                           WALL};

        Entity rightWall = {idCount++,
                            {219, 178, 219, 178},
                            (float)(i % MAX_ANIM_FRAMES),
                            ANIMATION_SPEED,
                            1,
                            0x87,
                            {0.0f, 0.0f},
                            {(float)(consoleWidth - 1), (float)i},
                            0.0f,
                            WALL};

        InsertEntityOnMatrix(&entityMatrix,
                             leftWall,
                             (uint8_t)leftWall.position[0],
                             (uint8_t)leftWall.position[1]);

        InsertEntityOnMatrix(&entityMatrix,
                             rightWall,
                             (uint8_t)rightWall.position[0],
                             (uint8_t)rightWall.position[1]);
    }
}

void UpdateObjectBehaviour()
{
    /* Atualiza cada objeto com base no seu comportamento.
     */

    PlayerBehaviour();

    for (uint8_t i = 0; i < entityMatrix.height; i++)
        for (uint8_t j = 0; j < entityMatrix.width; j++)
            if (GetEntityPtrFromMatrix(&entityMatrix, j, i)->type == ENEMY)
                EnemyBehaviour(j, i);
}

void PlayerBehaviour()
{
    /* Comportamento do jogador.
     */

    float runCoefficient = 1.0f; // Coeficiente de corrida

    if (GetKeyState(VK_SPACE) & 0x8000) // Espaço
        runCoefficient = 2.0f; // Dobra a velocidade na corrida

    if (GetKeyState(VK_UP) & 0x8000) // Seta para cima
        // Adiciona velocidade para cima
        entityMatrix.playerPtr->velocity[1] = -entityMatrix.playerPtr->speed * runCoefficient;
    else if (GetKeyState(VK_DOWN) & 0x8000) // Seta para baixo
        // Adiciona velocidade para baixo
        entityMatrix.playerPtr->velocity[1] = entityMatrix.playerPtr->speed * runCoefficient;

    if (GetKeyState(VK_RIGHT) & 0x8000) // Seta para a direita
        // Adiciona velocidade para direita
        entityMatrix.playerPtr->velocity[0] = entityMatrix.playerPtr->speed * runCoefficient;
    else if (GetKeyState(VK_LEFT) & 0x8000) // Seta para a esquerda
        // Adiciona velocidade para esquerda
        entityMatrix.playerPtr->velocity[0] = -entityMatrix.playerPtr->speed * runCoefficient;

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (entityMatrix.playerPtr->velocity[0] != 0.0f && entityMatrix.playerPtr->velocity[1] != 0.0f)
    {
        // Obtém a posição esperada
        uint8_t expectedPositionX = (uint8_t)(entityMatrix.playerPtr->position[0] +
                                              entityMatrix.playerPtr->velocity[0] / (float)tick);

        uint8_t expectedPositionY = (uint8_t)(entityMatrix.playerPtr->position[1] +
                                              entityMatrix.playerPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (uint8_t)entityMatrix.playerPtr->position[0] &&
            expectedPositionY != (uint8_t)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[1] = 0.0f;

        // Corrige a velocidade horizontal
        if (expectedPositionX != (uint8_t)entityMatrix.playerPtr->position[0] &&
            expectedPositionY == (uint8_t)entityMatrix.playerPtr->position[1])
            entityMatrix.playerPtr->velocity[0] = 0.0f;
    }
}

void EnemyBehaviour(uint8_t x, uint8_t y)
{
    /* Comportamento do inimigo.
     */

    Entity *enemyPtr = GetEntityPtrFromMatrix(&entityMatrix, x, y); // Ponteiro do inimigo

    if ((uint8_t)entityMatrix.playerPtr->position[0] > x) // Caso o jogador esteja na direita
        // Adiciona velocidade para cima
        enemyPtr->velocity[0] = enemyPtr->speed;
    else if ((uint8_t)entityMatrix.playerPtr->position[0] < x) // Caso o jogador esteja na esquerda
        // Adiciona velocidade para baixo
        enemyPtr->velocity[0] = -enemyPtr->speed;

    if ((uint8_t)entityMatrix.playerPtr->position[1] > y) // Caso o jogador esteja em cima
        // Adiciona velocidade para direita
        enemyPtr->velocity[1] = enemyPtr->speed;
    else if ((uint8_t)entityMatrix.playerPtr->position[1] < y) // Caso o jogador esteja em baixo
        // Adiciona velocidade para esquerda
        enemyPtr->velocity[1] = -enemyPtr->speed;

    /* Corrige a dessincronização diagonal (Fenômeno em que a posição horizontal e vertical
       avançam em passos diferentes.

       Este bloco é executado apenas quando o movimento é na diagonal.
    */
    if (enemyPtr->velocity[0] != 0.0f && enemyPtr->velocity[1] != 0.0f)
    {
        // Obtém a posição esperada
        uint8_t expectedPositionX = (uint8_t)(enemyPtr->position[0] +
                                              enemyPtr->velocity[0] / (float)tick);
        uint8_t expectedPositionY = (uint8_t)(enemyPtr->position[1] +
                                              enemyPtr->velocity[1] / (float)tick);

        // Corrige a velocidade vertical
        if (expectedPositionX == (uint8_t)enemyPtr->position[0] &&
            expectedPositionY != (uint8_t)enemyPtr->position[1])
            enemyPtr->velocity[1] = 0.0f;


        // Corrige a velocidade horizontal
        if (expectedPositionX != (uint8_t)enemyPtr->position[0] &&
            expectedPositionY == (uint8_t)enemyPtr->position[1])
            enemyPtr->velocity[0] = 0.0f;
    }
}

void UpdatePhysics()
{
    /* Atualiza a física e verifica os eventos do gameplay.
     */

    Entity *objectPtr;
    Entity *objectPtrInTargetPosition;
    Entity *objectPtrInCoinPosition;
    Entity *objectPtrInEnemyPosition;
    uint8_t oldPositionX;
    uint8_t oldPositionY;
    float targetPositionX;
    float targetPositionY;
    uint8_t oldCoinPositionX;
    uint8_t oldCoinPositionY;
    float coinSpawnX;
    float coinSpawnY;
    float enemySpawnX;
    float enemySpawnY;
    float distanceFromPlayer;

    for (uint8_t i = 0; i < entityMatrix.height; i++)
    {
        for (uint8_t j = 0; j < entityMatrix.width; j++)
        {
            // Obtém o ponteiro para o objeto
            objectPtr = GetEntityPtrFromMatrix(&entityMatrix, j, i);

            // Obtém a posição
            oldPositionX = (uint8_t)objectPtr->position[0];
            oldPositionY = (uint8_t)objectPtr->position[1];

            // Obtém a posição alvo
            targetPositionX = objectPtr->position[0] + objectPtr->velocity[0] / (float)tick;
            targetPositionY = objectPtr->position[1] + objectPtr->velocity[1] / (float)tick;

            // Redefine a velocidade
            objectPtr->velocity[0] = 0.0f;
            objectPtr->velocity[1] = 0.0f;

            // Ponteiro do objeto na posição alvo
            objectPtrInTargetPosition = GetEntityPtrFromMatrix(&entityMatrix,
                                                                (uint8_t)targetPositionX,
                                                                (uint8_t)targetPositionY);
            if (objectPtr->type == PLAYER) // Se for o jogador
            {
                /* Move o jogador caso o objeto na posição alvo esteja vazio ou seja o próprio
                    jogador
                */
                if (objectPtrInTargetPosition->type == EMPTY ||
                    objectPtrInTargetPosition->id == objectPtr->id)
                {
                    // Define a nova posição
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    // Move o jogador na matriz
                    MoveEntityOnMatrix(&entityMatrix,
                                        oldPositionX,
                                        oldPositionY,
                                        (uint8_t)objectPtr->position[0],
                                        (uint8_t)objectPtr->position[1]);
                }
                // Caso o jogador esteja pegando a moeda
                else if (objectPtrInTargetPosition->type == COIN)
                {
                    score++;

                    // Atualiza o texto da pontuação
                    sprintf(gameplay.texts[1].content, "score: %05d", score);
                    gameplay.texts[1].update = 1;

                    // Obtém a posição da moeda
                    oldCoinPositionX = (uint8_t)entityMatrix.coinPtr->position[0];
                    oldCoinPositionY = (uint8_t)entityMatrix.coinPtr->position[1];

                    /* Não é a maneira ideal de evitar a sobreposição da moeda em outros
                        objetos
                    */
                    do
                    {
                        /* Gera uma nova posição e repete o processo caso tenha algo na posição
                            gerada
                        */

                        coinSpawnX = Randomf(1, entityMatrix.width - 2);
                        coinSpawnY = Randomf(1, entityMatrix.height - 3);

                        objectPtrInCoinPosition = GetEntityPtrFromMatrix(&entityMatrix,
                                                                            (uint8_t)coinSpawnX,
                                                                            (uint8_t)coinSpawnY);
                    } while (objectPtrInCoinPosition->type != EMPTY);

                    entityMatrix.coinPtr->position[0] = coinSpawnX;
                    entityMatrix.coinPtr->position[1] = coinSpawnY;

                    // Move a moeda na matriz
                    MoveEntityOnMatrix(&entityMatrix,
                                        oldCoinPositionX,
                                        oldCoinPositionY,
                                        (uint8_t)entityMatrix.coinPtr->position[0],
                                        (uint8_t)entityMatrix.coinPtr->position[1]);

                    /* Gera a posição do novo inimigo. A posição não pode sobrepor nenhum
                        objeto e deve manter uma distância do jogador.
                    */
                    do
                    {
                        enemySpawnX = Randomf(1, entityMatrix.width - 2);
                        enemySpawnY = Randomf(1, entityMatrix.height - 3);

                        objectPtrInEnemyPosition = GetEntityPtrFromMatrix(&entityMatrix,
                                                                            (uint8_t)enemySpawnX,
                                                                            (uint8_t)enemySpawnY);

                        distanceFromPlayer = sqrtf(powf((enemySpawnX - targetPositionX), 2.0f) +
                                                    powf((enemySpawnY - targetPositionY), 2.0f));

                    } while (objectPtrInEnemyPosition->type != EMPTY || distanceFromPlayer < 20.0f);

                    Entity enemy = {idCount++,
                                    {178, 177, 176, 178},
                                    0.0f,
                                    ANIMATION_SPEED,
                                    1,
                                    0x0C,
                                    {0.0f, 0.0f},
                                    {enemySpawnX, enemySpawnY},
                                    ENEMY_SPEED,
                                    ENEMY};

                    // Insere o inimigo na matriz
                    InsertEntityOnMatrix(&entityMatrix, enemy, enemySpawnX, enemySpawnY);

                    // Move o jogador
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    MoveEntityOnMatrix(&entityMatrix,
                                        oldPositionX,
                                        oldPositionY,
                                        (uint8_t)objectPtr->position[0],
                                        (uint8_t)objectPtr->position[1]);
                }
            }
            else if (objectPtr->type == ENEMY) // Se for o inimigo
            {
                // Se move caso o a posição alvo não tenha um objeto ou tenha o próprio inimigo
                if (objectPtrInTargetPosition->type == EMPTY ||
                    objectPtrInTargetPosition->id == objectPtr->id)
                {
                    objectPtr->position[0] = targetPositionX;
                    objectPtr->position[1] = targetPositionY;

                    MoveEntityOnMatrix(&entityMatrix,
                                        oldPositionX,
                                        oldPositionY,
                                        (uint8_t)targetPositionX,
                                        (uint8_t)targetPositionY);
                }
                // Caso a posição tenha o jogador
                else if (objectPtrInTargetPosition->type == PLAYER)
                {
                    events[0] = GM_GAMEOVER; // Adiciona o evento de fim de jogo
                    sprintf(gameover.texts[1].content, "score: %05d", score);
                }
            }
        }
    }
}

void Render()
{
    /* Rederiza as entidades.
     */

    for (unsigned int i = 0; i < entityMatrix.height; i++)
    {
        for (unsigned int j = 0; j < entityMatrix.width; j++)
        {
            // Limpa o caractere caso ela esteja vazio na matriz
            if (GetEntityPtrFromMatrix(&entityMatrix, j, i)->type == EMPTY)
            {
                SetCharOnPosition(j, i, 32, 0x00);
            }
            else // Coloca o caractere
            {
                unsigned int newAnimationFrame;
                int isAnimated = GetEntityPtrFromMatrix(&entityMatrix, j, i)->isAnimated;

                if (isAnimated)
                {
                    GetEntityPtrFromMatrix(&entityMatrix, j, i)->animationFrame += ANIMATION_SPEED / (float)tick;

                    if ((unsigned int)GetEntityPtrFromMatrix(&entityMatrix, j, i)->animationFrame > MAX_ANIM_FRAMES - 1)
                        GetEntityPtrFromMatrix(&entityMatrix, j, i)->animationFrame = 0.0f;
                }

                newAnimationFrame = (unsigned int)GetEntityPtrFromMatrix(&entityMatrix, j, i)->animationFrame;

                char newChar = GetEntityPtrFromMatrix(&entityMatrix, j, i)->c[newAnimationFrame];

                SetCharOnPosition(j, i, newChar, GetEntityPtrFromMatrix(&entityMatrix, j, i)->color);
            }
        }
    }

    WriteOutput();
}

float Tick(double elapsedTime)
{
    /* Controla o tempo de espera entre frames e o retorna.
     */

    // Tempo necessário de espera em milisegundos
    int correctionTime = (int)(1000.0 / (double)tick - elapsedTime);

    if (correctionTime > 0) // Caso seja necessário esperar
    {
        Sleep(correctionTime); // Espera

        return 1000.0f / (float)tick;
    }
    else
    {
        return (float)elapsedTime;
    }
}

float Randomf(int min, int max)
{
    /* Gera um número aleatório no intervalo especificado. O número é um float porém é gerado como
       inteiro.
    */

    return (float)((rand() % (max - min + 1)) + min);
}
