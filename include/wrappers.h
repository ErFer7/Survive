#pragma once

#include "../include/core.h"
#include "../include/entity.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/vector2D.h"

void StartGameplay(GameplayContext *gameplayCtxPtr,
                   Vector2D size,
                   int fixedScreen,
                   int empty,
                   EventState *eventStateCtxPtr,
                   ThreadsContext *threadsCtxPtr,
                   ConsoleContext *consoleCtxPtr,
                   InterfaceContext *interfaceCtxPtr,
                   TimeContext *timeCtxPtr);
