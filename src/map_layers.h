/**
 * Copyright 2016 Alexander Scheel
 *
 * Map layers header file
**/

#include "map.h"

#pragma once
#ifndef MAP_LAYERS_H
#define MAP_LAYERS_H

void map_layers(map_c* current);
void map_layers_rooms(map_c* current);
void map_layers_hallways(map_c* current);

#endif
