// Sobreviva - ErFer7

/***
 *    ███████╗██╗   ██╗██████╗ ██╗   ██╗██╗██╗   ██╗███████╗
 *    ██╔════╝██║   ██║██╔══██╗██║   ██║██║██║   ██║██╔════╝
 *    ███████╗██║   ██║██████╔╝██║   ██║██║██║   ██║█████╗
 *    ╚════██║██║   ██║██╔══██╗╚██╗ ██╔╝██║╚██╗ ██╔╝██╔══╝
 *    ███████║╚██████╔╝██║  ██║ ╚████╔╝ ██║ ╚████╔╝ ███████╗
 *    ╚══════╝ ╚═════╝ ╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═══╝  ╚══════╝
 */

#include <pthread.h>

#include "../include/core.h"
#include "../include/entity.h"
#include "../include/graphics.h"
#include "../include/interface.h"
#include "../include/utilities.h"
#include "../include/vector2D.h"
#include "../include/world.h"
#include "../include/wrappers.h"

#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 30

int main() {
    EventState event_state;

    init_event_state(&event_state);

    while (event_state.state != EXIT) {
        // For every state: update_scene()

        switch (event_state.state) {
            case MAIN_MENU:
            case INFO_MENU:
            case START_MENU:
            case GENERATING:
            case GAMEPLAY:
            case PAUSING:
            case PAUSE:
            case RESUMING:
            case RESTARTING:
            case RETURNING:
            case GAMEOVER:
            case FINISHING:
            case EXITING:
            case EXIT:
            default:
                break;
        }
    }

    free_event_state(&event_state);

    return 0;
}
