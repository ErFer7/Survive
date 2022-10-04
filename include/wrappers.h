#pragma once

#include "../include/entity.h"
#include "../include/vector2D.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/core.h"

void StartGameplay(GameplayContext *gameplayCtxPtr,
                   Vector2D size,
                   int fixedScreen,
                   int empty,
                   EventStateContext *eventStateCtxPtr,
                   ThreadsContext *threadsCtxPtr,
                   ConsoleContext *consoleCtxPtr,
                   InterfaceContext *interfaceCtxPtr,
                   TimeContext *timeCtxPtr);
