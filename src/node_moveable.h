/**
 * Copyright 2016 Alexander Scheel
 *
 * Node Moveable header file
**/

#pragma once
#ifndef NODE_MOVEABLE_H
#define NODE_MOVEABLE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t order;
    bool is_player;
    int speed;
} moveable;

int32_t moveables_moveable(const void *key, const void *with);

#endif
