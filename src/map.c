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
    map_layers(current);
}

void map_blank(map *current)
{
    current->cols = 80;
    current->rows = 21;

    current->seed = utils_genseed();
    srand(current->seed);

    current->rooms_layer = malloc(sizeof(char) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        current->rooms_layer[y] = malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            current->rooms_layer[y][x] = ' ';
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
        int r_width = 5;
        int r_height = 4;

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
            room current = candidates[choices[i]];
            for (int j = 0; j < i; j++) {
                room past = candidates[choices[j]];

                has_intersection = room_overlaps(&current, &past);
                if (has_intersection) {
                    choices[i] = (((rand() + rand()) % r_rooms_generated) + r_rooms_generated) % r_rooms_generated;
                    break;
                }
            }

            tries += 1;

            if (tries >= r_rooms_generated) {
                i = 0;
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

void map_layers(map* current)
{
    map_layers_rooms(current);
    map_layers_hallways(current);
}

void map_layers_rooms(map* current)
{
    if (current->ready < 11387) {
        map_blank(current);
    }

    if (current->ready >= 12845) {
        return;
    }

    for (int i = 0; i < current->room_count; i++) {
        room current_room = current->rooms[i];

        for (int y = current_room.pos_y; y <= current_room.max_y; y++) {
            for (int x = current_room.pos_x; x < current_room.max_x; x++) {
                if (current->rooms_layer[y][x] != ' ') {
                    printf("%i,%i\n", x, y);
                }

                current->rooms_layer[y][x] = '.';
            }
        }
    }

    current->ready = 12845;
}

void map_layers_hallways(map* current)
{
    if (current->ready < 12845) {
        map_blank(current);
    }

    if (current->ready >= 13921) {
        return;
    }

    current->ready = 13921;
}

void map_print(map* current)
{
    if (current->ready < 13921) {
        map_blank(current);
    }
    printf("*--------------------------------------------------------------------------------*\n");
    for (int y = 0; y < current->rows; y++) {
        printf("|");
        for (int x = 0; x < current->cols; x++) {
            printf("%c", current->rooms_layer[y][x]);
        }
        printf("|\n");
    }
    printf("*--------------------------------------------------------------------------------*\n");
    printf("\n\n\n");
}
