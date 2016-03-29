/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy header file
**/


#pragma once
#ifndef MAP_ENEMY_H
#define MAP_ENEMY_H

#include "map.h"

void map_enemy_init(map_t* current);
void map_enemy_render(map_t* current);
void map_enemy_update_last_seen(map_t* current, int enemy);
void map_enemy_move(map_t* current, int enemy);
void map_enemy_deinit(map_t* current);

#endif
