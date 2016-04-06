/**
 * Copyright 2016 Alexander Scheel
 *
 * Object header file
**/

#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "map.h"
#include "dice.h"
#include <string>

using namespace std;

class object_t
{
public:
    string name;
    string description;
    string type;

    int color;

    int hit;
    dice_t damage;
    int dodge;
    int defense;
    int weight;
    int speed;
    int attr;
    int value;
};

class object_f
{
public:
    string name;
    string description;
    string type;
    string color;
    dice_t hit;
    dice_t damage;
    dice_t dodge;
    dice_t defense;
    dice_t weight;
    dice_t speed;
    dice_t attr;
    dice_t value;

    object_t* make_widget();
};

#endif
