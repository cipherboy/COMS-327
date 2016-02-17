/**
 * Copyright 2016 Alexander Scheel
 *
 * Point implementation file
**/

#include <stdint.h>

#include "point.h"

int32_t distances_distance(const void *key, const void *with)
{
    distances* a = (distances *) key;
    distances* b = (distances *) with;
    return a->distance - b->distance;
}
