/**
 * Copyright 2016 Alexander Scheel
 *
 * Map rooms implementation file
**/

#include <stdbool.h>
#include "map.h"
#include "room.h"

bool map_rooms_contains_point(map_c* current, int x, int y)
{
    for (int i = 0; i < current->room_count; i++) {
        if (room_contains_point(x, y, &current->rooms[i])) {
            return true;
        }
    }
    return false;
}

int map_rooms_find_contains_point(map_c* current, int x, int y)
{
    for (int i = 0; i < current->room_count; i++) {
        if (room_contains_point(x, y, &current->rooms[i])) {
            return i;
        }
    }

    return -1;
}
