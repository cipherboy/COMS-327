/**
 * Copyright 2016 Alexander Scheel
 *
 * Map layers header file
**/

#include <stdlib.h>
#include "map.h"

void map_layers(map_c* current)
{
    map_layers_rooms(current);
    map_layers_hallways(current);
}

void map_layers_rooms(map_c* current)
{
    for (int i = 0; i < current->room_count; i++) {
        room current_room = current->rooms[i];

        for (int y = current_room.pos_y; y <= current_room.max_y; y++) {
            for (int x = current_room.pos_x; x < current_room.max_x; x++) {
                current->rooms_layer[y][x] = '.';
                current->rock_hardness[y][x] = 0;
            }
        }
    }
}

void map_layers_hallways(map_c* current)
{
    // Adjacency matrix for rooms...
    bool connected[current->room_count][current->room_count];
    for (int i = 0; i < current->room_count; i++) {
        for (int j = 0; j < current->room_count; j++) {
            connected[i][j] = false;
        }
    }

    for (int i = 0; i < current->room_count; i++) {
        room current_room = current->rooms[i];

        for (int j = 0; j < current->room_count; j++) {
            if (connected[i][j]) {
                break;
            }

            room referenced_room = current->rooms[j];
            int cy = current_room.pos_y + (current_room.height/2);
            int cx = current_room.pos_x + (current_room.width/2);
            int ry = referenced_room.pos_y + (referenced_room.height/2);
            int rx = referenced_room.pos_x + (referenced_room.width/2);

            int direction_x = (rx-cx);
            if (direction_x < 0) {
                direction_x = -1;
            } else if (direction_x > 0) {
                direction_x = 1;
            }

            int direction_y = (ry-cy);
            if (direction_y < 0) {
                direction_y = -1;
            } else if (direction_y > 0) {
                direction_y = 1;
            }

            int y = cy;
            int x = cx;

            if (rand() % (current->room_count-2)) {
                for (x = cx; x != rx; x += direction_x) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
                for (y = cy; y != ry; y += direction_y) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
            } else {
                for (y = cy; y != ry; y += direction_y) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
                for (x = cx; x != rx; x += direction_x) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
            }

            connected[i][j] = true;
            connected[j][i] = true;
        }
    }
}
