/**
 * Copyright 2016 Alexander Scheel
 *
 * Main room structure header file
**/

#include <stdint.h>
#include "room.h"

#pragma once
#ifndef MAP_H
#define MAP_H

typedef struct
{
    short ready;
    char* path;

    int rows; // max_x
    int cols; // max_y

    unsigned int seed;

    uint8_t** rock_hardness;

    int room_count;
    room* rooms;
    char** rooms_layer;

    char** hallways_layer;
} map;

void map_init(map* current);
void map_read(map* current);
void map_blank(map* current);
void map_fill(map* current);
void map_layers(map* current);
void map_layers_rooms(map* current);
void map_layers_hallways(map* current);
void map_print(map* current);

#endif
