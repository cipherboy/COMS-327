/**
 * Copyright 2016 Alexander Scheel
**/

#include <linux/random.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "room.h"
#include "map.h"
#include "utils.h"

void map_init(map *current)
{
    map_blank(current);
    map_fill(current);
    map_layer(current);
}

void map_blank(map *current)
{
    current->cols = 80;
    current->rows = 21;

    current->seed = utils_genseed();
    srand(current->seed);

    current->rooms_layer = malloc(sizeof(char) * current->rows);
    for (int x = 0; x < current->rows; x++) {
        current->rooms_layer[x] = malloc(sizeof(char) * current->cols);
        for (int y = 0; y < current->cols; y++) {
            current->rooms_layer[x][y] = ' ';
        }
    }

    current->ready = 10321;
}

void map_fill(map *current)
{
    if (current->ready < 10321) {
        map_blank(current);
    }

    if (current->ready >= 11387) {
        return;
    }

    int r_rooms_generated = ((rand() % (5*current->rows*current->cols)) + (5*current->rows*current->cols)) % (5*current->rows*current->cols);
    r_rooms_generated += 3*current->rows*current->cols;

    room candidates[r_rooms_generated];

    // 3 <= pos_x <= 71
    // 2 <= pos_y <= 16
    // 3 <= width <= 15 ; pos_x + width < current->cols
    // 2 <= height <= 5 ; pos_y + height < current->rows

    for (int i = 0; i < r_rooms_generated; i++) {
        int r_pos_x = 3;
        int r_pos_y = 1;
        int r_width = 3;
        int r_height = 2;

        r_pos_x += rand() % 68;
        r_pos_y += rand() % 14;
        r_width += rand() % 12;
        r_height += rand() % 3;

        if (r_width + r_pos_x >= current->cols) {
            r_width = 3;
        }

        if (r_height + r_pos_y >= current->rows) {
            r_height = 2;
        }

        candidates[i].pos_x = r_pos_x;
        candidates[i].pos_y = r_pos_y;
        candidates[i].height = r_height;
        candidates[i].width = r_width;
        candidates[i].max_x = r_pos_x + r_width;
        candidates[i].max_y = r_pos_y + r_height;
    }

    // 5 <= r_num_rooms <= 8
    int r_num_rooms = 5;
    r_num_rooms += rand() % 3;

    int choices[r_num_rooms];

    for (int i = 0; i < r_num_rooms; i++) {
        bool has_intersection = false;
        int tries = 0;
        do {
            choices[i] = (((rand() + rand()) % r_rooms_generated) + r_rooms_generated) % r_rooms_generated;

            has_intersection = false;
            for (int j = 0; j < i; j++) {
                room current = candidates[choices[i]];
                room past = candidates[choices[j]];

                has_intersection = has_intersection || room_overlaps(&current, &past);
            }

            tries += 1;

            if (tries >= r_rooms_generated) {
                printf("Redoing last tile placement on rooms %i:%i with %i choices...\n", i, r_num_rooms, r_rooms_generated);
                i = -1;
                tries = 0;
                break;
            }
        } while (has_intersection);
    }

    current->room_count = r_num_rooms;
    current->rooms = malloc(sizeof(room) * r_num_rooms);

    for (int i = 0; i < r_num_rooms; i++) {
        current->rooms[i] = candidates[choices[i]];
    }

    current->ready = 11387;
}

void map_layer(map* current) {
    if (
}
