/**
 * Copyright 2016 Alexander Scheel
 *
 * Character header file
**/

#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "map.h"
#include "dice.h"
#include <string>

using namespace std;

class character_t
{
protected:
    map_c* current;
public:
    int pos_x;
    int pos_y;

    int hp;
    int speed;
    int color;
    dice_t attack_damage;

    bool is_alive;

    char representation;

    string name;
    string description;

    character_t(map_c* current);
};

class character_f
{
protected:
    map_c* current;
public:
    dice_t hp;
    dice_t attack_damage;

    char representation;

    string name;
    string description;
    string color;
    string abilities;

    dice_t speed_dice;

    character_f(map_c* current);

    enemy_t* make_widget();
};

#endif
