/**
 * Copyright 2016 Alexander Scheel
 *
 * Map monster header file
**/

#pragma once
#ifndef MAP_ITEM_READER_H
#define MAP_ITEM_READER_H

#include "object.h"
#include "map.h"

object_t** map_item_parse_file(map_t* current, char* basepath);

#endif
