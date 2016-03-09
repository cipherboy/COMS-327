/**
 * Copyright 2016 Alexander Scheel
 *
 * Map movement header file
**/

#include "map.h"

#pragma once
#ifndef MAP_MOVEMENT_H
#define MAP_MOVEMENT_H

int map_main(map* current, bool splash);
void map_render_splash();
void map_display_enemies(map* current);

#endif
