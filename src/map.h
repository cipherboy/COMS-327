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
class character_f;
class object_t;
class object_f;

class map_c
{
public:
    int rows; // max_x
    int cols; // max_y

    int monster_factory_count;
    int object_factory_count;
    character_f** monster_factories;
    object_f** object_factories;

    player_t* main_character;

    uint8_t** rock_hardness;

    int room_count;
    room* rooms;

    int enemy_count;
    enemy_t** enemies;

    int object_count;
    object_t** objects;

    object_t*** objects_location;
    character_t*** characters_location;

    char** characters_layer;
    char** rooms_layer;
    char** hallways_layer;
    char** char_buffer;

    int display_turn;
    char* display_message;
};

// Method declarations
#include "map_layers.h"
#include "map_rooms.h"
#include "map_player.h"
#include "map_enemy.h"
#include "map_enemy_move.h"
#include "map_object.h"
#include "map_movement.h"
#include "map_monster_reader.h"
#include "map_item_reader.h"

void map_init(map_c* current);
bool map_read(map_c* current, char* path);
void map_blank(map_c* current);
void map_fill(map_c* current);
void map_stairs(map_c* current);
void map_print(map_c* current);
void map_write(map_c* current, char* path);
void map_deinit(map_c* current);

#endif
