/**
 * Copyright 2016 Alexander Scheel
 *
 * Enemy implementation file
**/

#include "stdlib.h"
#include "stdbool.h"
#include "player.h"
#include "character.h"

player* player_new()
{
    return (player *) new player_t();
}

char*** player_player_distances(player* p)
{
    return &((*(player_t *)p).player_distances);
}

char*** player_all_distances(player* p)
{
    return &((*(player_t *)p).all_distances);
}

void player_delete(player* p)
{
    delete p;
}
