/**
 * Copyright 2016 Alexander Scheel
 *
 * Map layers header file
**/

#include "map.h"

#pragma once
#ifndef MAP_LAYERS_H
#define MAP_LAYERS_H

# ifdef __cplusplus
extern "C" {
# endif

void map_layers(map* current);
void map_layers_rooms(map* current);
void map_layers_hallways(map* current);

# ifdef __cplusplus
}
# endif

#endif
