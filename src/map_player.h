/**
 * Copyright 2016 Alexander Scheel
 *
 * Map player header file
**/


#pragma once
#ifndef MAP_PLAYER_H
#define MAP_PLAYER_H

void map_player_init(map_t* current);
void map_player_distances(map_t* current);
void map_player_distances_blank(map_t* current);
void map_player_distances_bounded(map_t* current);
void map_player_distances_unbounded(map_t* current);
void map_player_move(map_t* current, int dx, int dy);
void map_player_deinit(map_t* current);

#endif
