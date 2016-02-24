/**
 * Copyright 2016 Alexander Scheel
 *
 * Node Moveable implementation file
**/

#include <stdint.h>

#include "node_moveable.h"

int32_t moveables_moveable(const void *key, const void *with)
{
    moveable* a = (moveable *) key;
    moveable* b = (moveable *) with;
    
    if (a->speed != b->speed) {
        return a->speed - b->speed;
    }

    return a->order - b->order;
}
