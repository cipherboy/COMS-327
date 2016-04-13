/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "object.h"

class player_t : public character_t
{
public:
    char** player_distances;
    char** all_distances;
    char** seen_map;

    object_t* inventory;
    object_t** equipment;

    player_t(map_c* current);
    ~player_t();
};

#endif
