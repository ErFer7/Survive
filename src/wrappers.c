#include "../include/wrappers.h"

#include "../include/entity.h"
#include "../include/vector2D.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/core.h"
#include "../include/world.h"

void StartGameplay(GameplayContext *gameplayCtxPtr,
                   Vector2D size,
                   int fixedScreen,
                   int empty,
                   EventStateContext *eventStateCtxPtr,
                   ThreadsContext *threadsCtxPtr,
                   ConsoleContext *consoleCtxPtr,
                   InterfaceContext *interfaceCtxPtr,
                   TimeContext *timeCtxPtr)
                   {
                        // 120 caracteres é o limite do bom gosto né...

                        InitGameplayContext(gameplayCtxPtr, size, fixedScreen, empty);
                        GenerateWorld(gameplayCtxPtr);

                        StartUpdateThread(eventStateCtxPtr,
                                          gameplayCtxPtr,
                                          threadsCtxPtr,
                                          interfaceCtxPtr,
                                          timeCtxPtr);

                        StartRenderingThread(eventStateCtxPtr,
                                             gameplayCtxPtr,
                                             threadsCtxPtr,
                                             consoleCtxPtr,
                                             interfaceCtxPtr,
                                             timeCtxPtr);
                   }