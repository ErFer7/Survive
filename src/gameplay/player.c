#include "gameplay/player.h"

#include <notcurses/notcurses.h>

void handle_player_input(Entity *player, int key_a, int key_b) {
    if (key_a == NCKEY_UP || key_b == NCKEY_UP) {
        player->direction.y = -1;
    } else if (key_a == NCKEY_DOWN || key_b == NCKEY_DOWN) {
        player->direction.y = 1;
    }

    if (key_a == NCKEY_RIGHT || key_b == NCKEY_RIGHT) {
        player->direction.x = 1;
    } else if (key_a == NCKEY_LEFT || key_b == NCKEY_LEFT) {
        player->direction.x = -1;
    }
}
