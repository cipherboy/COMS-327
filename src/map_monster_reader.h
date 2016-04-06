/**
 * Copyright 2016 Alexander Scheel
 *
 * Map monster header file
**/

#pragma once
#ifndef MAP_MONSTER_READER_H
#define MAP_MONSTER_READER_H

#include "character.h"
#include "map.h"

character_f** map_monster_parse_file(map_c* current, char* basepath, int* count);

#endif
