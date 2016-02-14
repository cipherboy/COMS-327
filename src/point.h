#pragma once
#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
} point;

int32_t point_distance(const void *key, const void *with);

#endif
