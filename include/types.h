#pragma once

#include <stdint.h>

typedef enum Alignment Alignment;
typedef enum Status Status;
typedef enum Color Color;
typedef enum Key Key;
typedef enum GameplayTransitionMode GameplayTransitionMode;
typedef enum CellType CellType;

typedef uint32_t utf8_char;
typedef unsigned char byte;

typedef struct PeriodicThread PeriodicThread;
typedef struct InputState InputState;

typedef struct Text Text;
typedef struct Button Button;

typedef struct Cell Cell;

typedef struct Entity Entity;
typedef struct World World;
typedef struct Gameplay Gameplay;
typedef struct View View;

typedef struct Scene Scene;

typedef struct MenuScene MenuScene;
typedef struct InfoScene InfoScene;
typedef struct StartScene StartScene;
typedef struct GameplayScene GameplayScene;
typedef struct PauseScene PauseScene;
typedef struct GameoverScene GameoverScene;

typedef struct GameplaySceneArgs GameplaySceneArgs;

typedef struct SceneContext SceneContext;
typedef struct SceneTransition SceneTransition;
