/**
 * Copyright 2016 Alexander Scheel
 *
 * Main map header file
**/

#pragma once
#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include "player.h"
#include "enemy.h"
#include "room.h"

typedef struct {
    int rows; // max_x
    int cols; // max_y

    player main_character;

    uint8_t** rock_hardness;

    int room_count;
    room* rooms;

    int enemy_count;
    enemy* enemies;

    void*** characters_location;

    char** characters_layer;
    char** rooms_layer;
    char** hallways_layer;
    char** char_buffer;
} map;

// Method declarations
#include "map_layers.h"
#include "map_rooms.h"
#include "map_player.h"
#include "map_enemy.h"
#include "map_enemy_move.h"
#include "map_movement.h"

void map_init(map* current);
bool map_read(map* current, char* path);
void map_blank(map* current);
void map_fill(map* current);
void map_stairs(map* current);
void map_print(map* current);
void map_write(map* current, char* path);
void map_deinit(map* current);


#endif
