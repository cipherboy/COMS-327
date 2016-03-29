/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H


#include "character.h"

typedef character player;

player* player_new();
char*** player_player_distances(player* p);
char*** player_all_distances(player* p);
char*** player_seen_map(player* p);
void player_delete(player* p);


class player_t : public character_t
{
public:
    char** player_distances;
    char** all_distances;
    char** seen_map;
};

#endif
