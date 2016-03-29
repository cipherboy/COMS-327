/**
 * Copyright 2016 Alexander Scheel
 *
 * Node Distances implementation file
**/

#include <stdint.h>

#include "node_distances.h"

int32_t distances_distance(const void *key, const void *with)
{
    distances* a = (distances *) key;
    distances* b = (distances *) with;
    return a->distance - b->distance;
}
