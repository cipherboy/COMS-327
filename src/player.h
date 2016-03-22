/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H


#ifdef __cplusplus
extern "C" {
#endif
#include "character.h"

typedef character player;

player* player_new();
char*** player_player_distances(player* p);
char*** player_all_distances(player* p);
void player_delete(player* p);

#ifdef __cplusplus
}

class player_t : public character_t
{
public:
    char** player_distances;
    char** all_distances;
};

#endif
#endif
