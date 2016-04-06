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

object_f** map_item_parse_file(map_c* current, char* basepath, int* count);

#endif
