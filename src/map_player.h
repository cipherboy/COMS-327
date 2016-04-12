/**
 * Copyright 2016 Alexander Scheel
 *
 * Map player header file
**/


#pragma once
#ifndef MAP_PLAYER_H
#define MAP_PLAYER_H

class player_t;
class character_t;

void map_player_init(map_c* current, player_t* player);
void map_player_distances(map_c* current);
void map_player_distances_blank(map_c* current);
void map_player_distances_bounded(map_c* current);
void map_player_distances_unbounded(map_c* current);
void map_player_move(map_c* current, int dx, int dy);
void map_player_deinit(map_c* current);

#endif
