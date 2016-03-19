/**
 * Copyright 2016 Alexander Scheel
 *
 * Map player header file
**/


#pragma once
#ifndef MAP_PLAYER_H
#define MAP_PLAYER_H

# ifdef __cplusplus
extern "C" {
# endif

void map_player_init(map* current);
void map_player_distances(map* current);
void map_player_distances_blank(map* current);
void map_player_distances_bounded(map* current);
void map_player_distances_unbounded(map* current);
void map_player_move(map* current, int dx, int dy);
void map_player_deinit(map* current);

# ifdef __cplusplus
}
# endif

#endif
