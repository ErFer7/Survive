#pragma once

#include "../include/core.h"
#include "../include/entity.h"

void InitEventStateMachine(EventStateContext *eventStateCtxPtr,
                           ConsoleContext *consoleCtxPtr,
                           float tick_,
                           int consoleWidth,
                           int consoleHeight);
void RunEventStateMachine(EventStateContext *eventStateCtxPtr, ConsoleContext *consoleCtxPtr);
void FreeEventStateMachine(EventStateContext *eventStateCtxPtr, ConsoleContext *consoleCtxPtr);
