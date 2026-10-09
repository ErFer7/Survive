#include "gameplay/player.h"

#include <notcurses/notcurses.h>

#include "system/input.h"

void handle_player_input(Entity *player, InputState *input_state) {
    if (is_key_pressed(input_state, KEY_UP)) {
        player->direction.y = -1;
    } else if (is_key_pressed(input_state, KEY_DOWN)) {
        player->direction.y = 1;
    }

    if (is_key_pressed(input_state, KEY_RIGHT)) {
        player->direction.x = 1;
    } else if (is_key_pressed(input_state, KEY_LEFT)) {
        player->direction.x = -1;
    }

    if (is_key_pressed(input_state, KEY_SHIFT)) {
        player->speed_modifier = 0.5f;
    } else if (is_key_pressed(input_state, KEY_CTRL)) {
        player->speed_modifier = 1.5f;
    } else {
        player->speed_modifier = 1.0f;
    }
}
