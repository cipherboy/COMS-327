/**
 * Copyright 2016 Alexander Scheel
 *
 * Player header file
**/

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class object_t;

const char player_equipment_slot_names[][8] = {"WEAPON", "OFFHAND", "RANGED", "ARMOR", "HELMET", "CLOAK", "GLOVES", "BOOTS", "AMULET", "LIGHT", "RING", "RING"};


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
