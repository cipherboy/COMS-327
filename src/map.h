/**
 * Copyright 2016 Alexander Scheel
 *
 * Main map header file
**/



#pragma once
#ifndef MAP_H
#define MAP_H

#include "room.h"

#include <stdint.h>

class player_t;
class enemy_t;
class character_t;

class map_t
{
public:
    int rows; // max_x
    int cols; // max_y

    player_t* main_character;

    uint8_t** rock_hardness;

    int room_count;
    room* rooms;

    int enemy_count;
    enemy_t** enemies;

    character_t*** characters_location;

    char** characters_layer;
    char** rooms_layer;
    char** hallways_layer;
    char** char_buffer;
};

// Method declarations
#include "map_layers.h"
#include "map_rooms.h"
#include "map_player.h"
#include "map_enemy.h"
#include "map_enemy_move.h"
#include "map_movement.h"

void map_init(map_t* current);
bool map_read(map_t* current, char* path);
void map_blank(map_t* current);
void map_fill(map_t* current);
void map_stairs(map_t* current);
void map_print(map_t* current);
void map_write(map_t* current, char* path);
void map_deinit(map_t* current);

#endif
