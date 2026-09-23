#pragma once

// UI: User interface
// IN: Internal
// GP: Gameplay
enum Event {
    UI_START,
    UI_GENERATE,
    UI_INFO,
    UI_QUIT,
    UI_PAUSE,
    UI_RESUME,
    UI_RESTART,
    UI_RETURN,
    IN_GENERATED,
    IN_RESTARTED,
    IN_PAUSED,
    IN_RESUMED,
    GP_GAMEOVER,
    IN_FINISHED,
    IN_FREED
};
