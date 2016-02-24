/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy header file
**/


#pragma once
#ifndef MAP_ENEMY_H
#define MAP_ENEMY_H

#include "map.h"

void map_enemy_init(map* current);
void map_enemy_render(map* current);
void map_enemy_move(map* current, int enemy);
void map_enemy_deinit(map* current);

#endif
