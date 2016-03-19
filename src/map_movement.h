/**
 * Copyright 2016 Alexander Scheel
 *
 * Map movement header file
**/

#include "map.h"

#pragma once
#ifndef MAP_MOVEMENT_H
#define MAP_MOVEMENT_H

# ifdef __cplusplus
extern "C" {
# endif

int map_main(map* current, bool splash);
void map_render_splash();
void map_display_enemies(map* current);

# ifdef __cplusplus
}
# endif

#endif
