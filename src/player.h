/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

typedef struct {
    int pos_x;
    int pos_y;

    int speed;

    char** player_distances;
    char** all_distances;
} player;

#endif
