/**
 * Copyright 2016 Alexander Scheel
 *
 * Map rooms header file
**/

#include "map.h"

#pragma once
#ifndef MAP_ROOMS_H
#define MAP_ROOMS_H

bool map_rooms_contains_point(map_c* current, int x, int y);
int map_rooms_find_contains_point(map_c* current, int x, int y);

#endif
