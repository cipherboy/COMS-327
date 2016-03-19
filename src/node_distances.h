/**
 * Copyright 2016 Alexander Scheel
 *
 * Point header file
**/

#pragma once
#ifndef NODE_DISTANCES_H
#define NODE_DISTANCES_H

#include <stdint.h>

typedef struct {
    int32_t x;
    int32_t y;
    int32_t distance;
} distances;

# ifdef __cplusplus
extern "C" {
# endif

int32_t distances_distance(const void *key, const void *with);

# ifdef __cplusplus
}
# endif

#endif
