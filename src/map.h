/**
 * Copyright 2016 Alexander Scheel
 *
 * Main room structure header file
**/

#pragma once
#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "player.h"
#include "room.h"

typedef struct {
    int rows; // max_x
    int cols; // max_y

    player character;

    uint8_t** rock_hardness;

    int room_count;
    room* rooms;
    char** rooms_layer;

    char** hallways_layer;

    char** player_distances;
    char** all_distances;
} map;

void map_init(map* current);
void map_read(map* current, char* path);
void map_blank(map* current);
void map_fill(map* current);
void map_layers(map* current);
void map_layers_rooms(map* current);
void map_layers_hallways(map* current);
void map_print(map* current);
void map_write(map* current, char* path);
void map_deinit(map* current);

void map_player_init(map* current);
void map_player_distances(map* current);
void map_player_wall_distances(map* current);
void map_player_entire_distances(map* current);

#endif
