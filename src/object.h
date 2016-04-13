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

    int pos_x;
    int pos_y;

    int color;
    char representation;

    bool display;

    int stack_size;
    object_t** stack;
    bool no_recursive;

    int hit;
    dice_t damage;
    int dodge;
    int defense;
    int weight;
    int speed;
    int attr;
    int value;

    void convert_to_stack();
    bool convert_from_stack();
    bool add_to_stack(object_t* obj);
    object_t* pick_from_top_of_stack();
    object_t();
    ~object_t();
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
