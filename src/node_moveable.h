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
    int32_t speed;
    int32_t next_turn;
} moveable;

# ifdef __cplusplus
extern "C" {
# endif

int32_t moveables_moveable(const void *key, const void *with);

# ifdef __cplusplus
}
# endif

#endif
