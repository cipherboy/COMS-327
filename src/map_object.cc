
/**
 * Copyright 2016 Alexander Scheel
 *
 * Map objects implementation file
**/

#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "object.h"
#include "map_object.h"
#include "utils.h"

void map_objects_init(map_c* current)
{
    current->object_count = 15 + (rand() % 15);

    current->objects = (object_t**) malloc(sizeof(object_t*) * current->object_count);
    for (int i = 0; i < current->object_count; i++) {
        int factory = rand() % current->object_factory_count;
        current->objects[i] = current->object_factories[factory]->make_widget();
    }

    for (int i = 0; i < current->object_count; i++) {
        int room = rand() % current->room_count;
        int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
        int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

        current->objects[i]->pos_x = pos_x;
        current->objects[i]->pos_y = pos_y;

        current->objects_location[pos_y][pos_x] = current->objects[i];
    }
}

void map_objects_deinit(map_c* current)
{
    for (int i = 0; i < current->object_count; i++) {
        delete current->objects[i];
    }
    free(current->objects);
}
