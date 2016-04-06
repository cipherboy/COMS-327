/**
 * Copyright 2016 Alexander Scheel
 *
 * Character implementation file
**/


#include "stdlib.h"
#include "character.h"
#include "map.h"

character_t::character_t(map_c* current)
{
    this->current = current;

    int room = rand() % current->room_count;
    this->speed = 10;

    int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
    int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

    this->pos_x = pos_x;
    this->pos_y = pos_y;

    this->is_alive = true;
    this->representation = '@';
}

character_f::character_f(map_c* current)
{
    this->current = current;
    this->representation = '`';
}
