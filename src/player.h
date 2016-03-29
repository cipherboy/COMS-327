/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"

class player_t : public character_t
{
public:
    char** player_distances;
    char** all_distances;
    char** seen_map;

    player_t(map* current);
    ~player_t();
};

#endif
