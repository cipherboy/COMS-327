/**
 * Copyright 2016 Alexander Scheel
**/

#include <stdbool.h>

#pragma once
#ifndef ROOM_H
#define ROOM_H

typedef struct
{
    int pos_x;
    int pos_y;

    int max_x;
    int max_y;

    int width;
    int height;
} room;

bool room_overlaps(room* first, room* second);
bool room_contains_room(int x, int y, room* place);

#endif
