/**
 * Copyright 2016 Alexander Scheel
**/

#include <stdbool.h>
#include <stdint.h>

#pragma once
#ifndef ROOM_H
#define ROOM_H

typedef struct
{
    uint8_t pos_x;
    uint8_t pos_y;

    uint8_t width;
    uint8_t height;

    uint8_t max_x;
    uint8_t max_y;
} room;

bool room_overlaps(room* first, room* second);
bool room_contains_room(int x, int y, room* place);
int room_lazy_distance(room* first, room* second);

#endif
