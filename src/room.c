/**
 * Copyright 2016 Alexander Scheel
**/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "room.h"
#include "map.h"

bool room_overlaps(room* first, room* second)
{
    for (uint8_t y = first->pos_y; y <= first->max_y; y++) {
        for (uint8_t x = first->pos_x; x <= first->max_x; x++) {
            if (room_contains_point_overlap(x, y, second, 5)) {
                return true;
            }
        }
    }

    return false;
}

bool room_contains_point(uint8_t x, uint8_t y, room* place)
{
    return room_contains_point_overlap(x, y, place, 0);
}

bool room_contains_point_overlap(uint8_t x, uint8_t y, room* place, int overlap)
{
    if ((x >= place->pos_x-overlap && x <= place->max_x+overlap) && (y >= place->pos_y-overlap && y <= place->max_y+overlap)) {
        return true;
    }

    return false;
}

int room_lazy_distance(room* first, room* second)
{
    int min_distance = pow(80, 2) + pow(21, 2); // Long diagonal of map

    int fy = first->pos_y + (first->height/2);
    int fx = first->pos_x + (first->width/2);
    int sy = second->pos_y + (second->height/2);
    int sx = second->pos_x + (second    ->width/2);

    min_distance = pow((sx - fx), 2) + pow((sy - fy), 2);

    return (int) min_distance; // return squared distance
}
