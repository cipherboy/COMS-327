/**
 * Copyright 2016 Alexander Scheel
 *
 * Map movement header file
**/

#include "map.h"

#pragma once
#ifndef MAP_MOVEMENT_H
#define MAP_MOVEMENT_H

int map_main(map_c* current, bool splash);
void map_render_splash();
void map_display_enemies(map_c* current);
void map_display_inventory(map_c* current);
void map_display_equipment(map_c* current);
void map_display_item_description(map_c* current);

#endif
