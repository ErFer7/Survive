#pragma once

void InitEventStateMachine(float tick_, int consoleWidth_, int consoleHeight_);
void RunEventStateMachine();
void FreeEventStateMachine();
void PlayEventBehaviourWrapper(int worldWidth_, int worldHeight_, int empty_, int fixedScreen_);