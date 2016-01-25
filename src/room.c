/**
 * Copyright 2016 Alexander Scheel
**/

#include <linux/random.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "room.h"
#include "map.h"

bool room_overlaps(room* first, room* second)
{
    return room_contains(first->pos_x, first->pos_y, second) || room_contains(first->max_x, first->max_y, second) || false;
}

bool room_contains(int x, int y, room* place)
{
    if ((x >= place->pos_x && x <= place->max_x) && (y >= place->pos_y && y <= place->max_y)) {
        return true;
    }

    return false;
}
