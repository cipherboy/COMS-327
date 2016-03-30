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

character_t** map_monster_parse_file(map_t* current, char* basepath);

#endif
