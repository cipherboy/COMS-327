/**
 * Copyright 2016 Alexander Scheel
 *
 * Character header file
**/

#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "stdbool.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef double character; // for increased accuracy

character* character_new();
int* character_pos_x(character* c);
int* character_pos_y(character* c);
bool* character_is_alive(character* c);
char* character_representation(character* c);
int* character_speed(character* c);

# ifdef __cplusplus
}

class character_t
{
public:
    int pos_x;
    int pos_y;

    bool is_alive;

    char representation;

    int speed;
};

# endif

#endif
