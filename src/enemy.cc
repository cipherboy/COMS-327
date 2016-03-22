/**
 * Copyright 2016 Alexander Scheel
 *
 * Enemy implementation file
**/


#include "stdlib.h"
#include "stdbool.h"
#include "enemy.h"
#include "character.h"

enemy* enemy_new()
{
    return (enemy *) new enemy_t();
}

int* enemy_attributes(enemy* e)
{
    return &((*(enemy_t *)e).attributes);
}

bool* enemy_has_seen_main_character(enemy* e)
{
    return &((*(enemy_t *)e).has_seen_main_character);
}

int* enemy_main_character_last_x(enemy* e)
{
    return &((*(enemy_t *)e).main_character_last_x);
}

int* enemy_main_character_last_y(enemy* e)
{
    return &((*(enemy_t *)e).main_character_last_y);
}

void enemy_delete(enemy* e)
{
    delete e;
}
