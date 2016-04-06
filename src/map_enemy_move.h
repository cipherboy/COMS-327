/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy move header file
**/


#pragma once
#ifndef MAP_ENEMY_MOVE_H
#define MAP_ENEMY_MOVE_H

#include "map.h"

void map_enemy_move_erratic(map_c* current, int enemy_loc);
void map_enemy_move_random(map_c* current, int enemy_loc);
void map_enemy_move_intelligent_not_telepathic(map_c* current, int enemy_loc);
void map_enemy_move_not_intelligent_telepathic(map_c* current, int enemy_loc);
void map_enemy_move_intelligent_telepathic(map_c* current, int enemy_loc);
void map_enemy_move_not_intelligent_not_telepathic(map_c* current, int enemy_loc);

#endif
