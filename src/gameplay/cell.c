#include "gameplay/cell.h"

#include "gameplay/enemy.h"
#include "gameplay/player.h"
#include "gameplay/world.h"

void update_cell_character(Cell *cell) {
    uint32_t animation_length;

    switch (cell->type) {
        case PLAYER:
            animation_length = PLAYER_ANIMATION_LENGTH;
            break;
        case ENEMY:
            animation_length = ENEMY_ANIMATION_LENGTH;
            break;
        case COIN:
            animation_length = COIN_ANIMATION_LENGTH;
            break;
        case WALL:
            animation_length = WALL_ANIMATION_LENGTH;
            break;
        default:
            animation_length = 1;
            break;
    }

    cell->cell = (nccell)NCCELL_INITIALIZER(cell->characters[(uint32_t)cell->animation_index % animation_length],
                                            cell->cell.stylemask,
                                            cell->cell.channels);
}
