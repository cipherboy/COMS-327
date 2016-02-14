#include <stdlib.h>
#include "player.h"
#include "map.h"

void player_init(map* current)
{
    int room = rand() % current->room_count;

    int pos_x = current->rooms[room].pos_x + rand() % current->rooms[room].width;
    int pos_y = current->rooms[room].pos_y + rand() % current->rooms[room].height;

    current->character.pos_x = pos_x;
    current->character.pos_y = pos_y;
}

void player_distances(map* current)
{
}

void player_wall_distances(map* current)
{
}

void player_entire_distances(map* current)
{
}
