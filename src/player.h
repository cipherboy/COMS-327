/**
 * Copyright 2016 Alexander Scheel
 *
 * Main room structure header file
**/

#include <stdint.h>

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int pos_x;
    int pos_y;
} player;

void player_init(map* current);
void player_distances(map* current);
void player_wall_distances(map* current);
void player_entire_distances(map* current);

#endif
