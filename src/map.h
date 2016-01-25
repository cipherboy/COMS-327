/**
 * Copyright 2016 Alexander Scheel
 *
 * Main room structure header file
**/

#include <stdlib.h>
#include <linux/random.h>

#include "room.h"

#pragma once
#ifndef MAP_H
#define MAP_H

typedef struct
{
    short ready;

    int rows; // max_x
    int cols; // max_y
    
    unsigned int seed;

    int room_count;
    room* rooms;
    char** rooms_layer;
} map;

void map_init(map *current);
void map_blank(map *current);
void map_fill(map *current);


#endif
