/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy move header file
**/


#pragma once
#ifndef MAP_ENEMY_MOVE_H
#define MAP_ENEMY_MOVE_H

#include "map.h"

# ifdef __cplusplus
extern "C" {
# endif

void map_enemy_move_erratic(map* current, int enemy_loc);
void map_enemy_move_random(map* current, int enemy_loc);
void map_enemy_move_intelligent_not_telepathic(map* current, int enemy_loc);
void map_enemy_move_not_intelligent_telepathic(map* current, int enemy_loc);
void map_enemy_move_intelligent_telepathic(map* current, int enemy_loc);
void map_enemy_move_not_intelligent_not_telepathic(map* current, int enemy_loc);

# ifdef __cplusplus
}
# endif

#endif
