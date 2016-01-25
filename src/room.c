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
    for (int y = first->pos_y; y <= first->max_y; y++) {
        for (int x = first->pos_x; x <= first->max_x; x++) {
            if (room_contains_room(x, y, second)) {
                return true;
            }
        }
    }

    return false;
}

bool room_contains_room(int x, int y, room* place)
{
    int overlap = 5;

    if ((x >= place->pos_x-overlap && x <= place->max_x+overlap) && (y >= place->pos_y-overlap && y <= place->max_y+overlap)) {
        return true;
    }

    return false;
}
