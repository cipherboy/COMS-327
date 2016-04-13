
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
    current->object_count = 35 + (rand() % 15);

    current->objects = (object_t**) malloc(sizeof(object_t*) * 1);
    current->objects[0] = NULL;

    int added = 1;
    for (int i = 0; i < current->object_count; i++) {

        int factory = rand() % current->object_factory_count;
        object_t* object = current->object_factories[factory]->make_widget();

        int room = rand() % current->room_count;
        int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
        int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

        object->pos_x = pos_x;
        object->pos_y = pos_y;

        if (current->objects_location[pos_y][pos_x] == NULL) {
            current->objects[added-1] = object;
            current->objects_location[pos_y][pos_x] = object;

            added += 1;
            current->objects = (object_t**) realloc(current->objects, sizeof(object_t*) * (added));
            current->objects[added-1] = NULL;
        } else {
            if (!current->objects_location[pos_y][pos_x]->add_to_stack(object)) {
                printf("Error adding object to stack...\n");
            }
        }
    }

    current->object_count = added;
}

void map_objects_deinit(map_c* current)
{
    for (int i = 0; i < current->object_count; i++) {
        if (current->objects[i] != NULL) {
            delete current->objects[i];
        }
    }

    free(current->objects);
}
