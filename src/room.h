/**
 * Copyright 2016 Alexander Scheel
 *
 * Room header file
**/

#pragma once
#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t pos_x;
    uint8_t pos_y;

    uint8_t width;
    uint8_t height;

    uint8_t max_x;
    uint8_t max_y;
} room;

# ifdef __cplusplus
extern "C" {
# endif

bool room_overlaps(room* first, room* second);
bool room_contains_point(uint8_t x, uint8_t y, room* place);
bool room_contains_point_overlap(uint8_t x, uint8_t y, room* place, int overlap);
int room_lazy_distance(room* first, room* second);

# ifdef __cplusplus
}
# endif

#endif
