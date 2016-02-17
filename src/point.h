/**
 * Copyright 2016 Alexander Scheel
 *
 * Point header file
**/

#pragma once
#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct {
    int32_t x;
    int32_t y;
    int32_t distance;
} distances;

int32_t distances_distance(const void *key, const void *with);

#endif
