/**
 * Copyright 2016 Alexander Scheel
 *
 * Character implementation file
**/


#include "stdlib.h"
#include "character.h"

character* character_new()
{
    return (character *) malloc(sizeof(character_t));
}

int* character_pos_x(character* c)
{
    return &((*(character_t *)c).pos_x);
}

int* character_pos_y(character* c)
{
    return &((*(character_t *)c).pos_y);
}

bool* character_is_alive(character* c)
{
    return &((*(character_t *)c).is_alive);
}

char* character_representation(character* c)
{
    return &((*(character_t *)c).representation);
}

int* character_speed(character* c)
{
    return &((*(character_t *)c).speed);
}
