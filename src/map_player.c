/**
 * Copyright 2016 Alexander Scheel
 *
 * Map player implementation file
**/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "player.h"
#include "map.h"
#include "utils.h"
#include "node_distances.h"
#include "binheap.h"

void map_player_init(map* current)
{
    current->main_character = player_new();

    int room = rand() % current->room_count;
    (*character_speed(current->main_character)) = 10;

    int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
    int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

    (*character_pos_x(current->main_character)) = pos_x;
    (*character_pos_y(current->main_character)) = pos_y;

    (*character_is_alive(current->main_character)) = true;
    (*character_representation(current->main_character)) = '@';

    (*player_player_distances(current->main_character)) = malloc(sizeof(char*) * current->rows * current->cols);
    (*player_all_distances(current->main_character)) = malloc(sizeof(char*) * current->rows * current->cols);
    (*player_seen_map(current->main_character)) = malloc(sizeof(char*) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        ((*player_player_distances(current->main_character))[y]) = malloc(sizeof(char) * current->cols);
        ((*player_all_distances(current->main_character))[y]) = malloc(sizeof(char) * current->cols);
        ((*player_seen_map(current->main_character))[y]) = malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            ((*player_seen_map(current->main_character))[y][x]) = ' ';
        }
    }
}

void map_player_distances(map* current)
{
    map_player_distances_blank(current);
    map_player_distances_bounded(current);
    map_player_distances_unbounded(current);
}

void map_player_distances_blank(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            ((*player_player_distances(current->main_character))[y][x]) = 255;
            ((*player_all_distances(current->main_character))[y][x]) = 255;
        }
    }
}

void map_player_distances_bounded(map* current)
{
    binheap_t queue;
    binheap_init(&queue, distances_distance, NULL);

    distances** objects = malloc(sizeof(distances*) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = malloc(sizeof(binheap_node_t**) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = malloc(sizeof(binheap_node_t*) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != (*character_pos_y(current->main_character) )|| x != (*character_pos_x(current->main_character)) ) {
                objects[y][x].distance = 255;
            } else {
                objects[y][x].distance = 0;
            }
            bh_ptr[y][x] = binheap_insert(&queue, &objects[y][x]);
        }
    }

    distances* c;

    while ((c = (distances*) binheap_remove_min(&queue))) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int x = c->x + dx;
                int y = c->y + dy;
                int dist = 1 + objects[c->y][c->x].distance;

                if ((dx != dy) && x > 0 && y > 0 && x < current->cols && y < current->rows) {
                    if ((map_rooms_contains_point(current, x, y) || current->hallways_layer[y][x] == '#') && (dist < objects[y][x].distance || objects[y][x].distance < 0)) {
                        objects[y][x].distance = dist;
                        binheap_decrease_key(&queue, bh_ptr[y][x]);
                    }
                }
            }
        }
    }

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            if (current->rooms_layer[y][x] != ' ' || current->hallways_layer[y][x] != ' ') {
                ((*player_player_distances(current->main_character))[y][x]) = objects[y][x].distance;
            }
        }
    }

    for (int y = 0; y < current->rows; y++) {
        free(objects[y]);
        free(bh_ptr[y]);
    }
    free(objects);
    free(bh_ptr);
    binheap_delete(&queue);
}

void map_player_distances_unbounded(map* current)
{
    binheap_t queue;
    binheap_init(&queue, distances_distance, NULL);

    distances** objects = malloc(sizeof(distances*) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = malloc(sizeof(binheap_node_t**) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = malloc(sizeof(binheap_node_t*) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != (*character_pos_y(current->main_character) )|| x != (*character_pos_x(current->main_character)) ) {
                objects[y][x].distance = 255;
            } else {
                objects[y][x].distance = 0;
            }
            bh_ptr[y][x] = binheap_insert(&queue, &objects[y][x]);
        }
    }

    distances* c;

    while ((c = (distances*) binheap_remove_min(&queue))) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int x = c->x + dx;
                int y = c->y + dy;

                if ((dx != dy) && x > 0 && y > 0 && x < current->cols && y < current->rows) {
                    int dist = (1 + (double) current->rock_hardness[y][x] / 63.9) + objects[c->y][c->x].distance;
                    if (current->rock_hardness[y][x] != 255 && (dist < objects[y][x].distance || objects[y][x].distance < 0)) {
                        objects[y][x].distance = dist;
                        binheap_decrease_key(&queue, bh_ptr[y][x]);
                    }
                }
            }
        }
    }

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            ((*player_all_distances(current->main_character))[y][x]) = objects[y][x].distance;
        }
    }

    for (int y = 0; y < current->rows; y++) {
        free(objects[y]);
        free(bh_ptr[y]);
    }
    free(objects);
    free(bh_ptr);
    binheap_delete(&queue);
}

void map_player_move(map* current, int dx, int dy)
{
    int pos_x = (*character_pos_x(current->main_character));
    int pos_y = (*character_pos_y(current->main_character));

    map_enemy_render(current);

    pos_x = (*character_pos_x(current->main_character)) + dx;
    pos_y = (*character_pos_y(current->main_character)) + dy;

    while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        pos_x = (*character_pos_x(current->main_character)) + dx;
        pos_y = (*character_pos_y(current->main_character)) + dy;
    }

    if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
        if (current->rock_hardness[pos_y][pos_x] > 85) {
            current->rock_hardness[pos_y][pos_x] -= 85;
        } else {
            current->rock_hardness[pos_y][pos_x] = 0;
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->main_character) )!= pos_x || (*character_pos_y(current->main_character) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->main_character)) = pos_x;
            (*character_pos_y(current->main_character)) = pos_y;
        }

        map_player_distances(current);
    } else if (current->rock_hardness[pos_y][pos_x] == 0) {
        current->hallways_layer[pos_y][pos_x] = '#';

        if (((*character_pos_x(current->main_character) )!= pos_x || (*character_pos_y(current->main_character) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
            (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
        }

        (*character_pos_x(current->main_character)) = pos_x;
        (*character_pos_y(current->main_character)) = pos_y;
    }
}

void map_player_deinit(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        free(((*player_player_distances(current->main_character))[y]));
        free(((*player_all_distances(current->main_character))[y]));
        free(((*player_seen_map(current->main_character))[y]));
    }
    free((*player_player_distances(current->main_character)));
    free((*player_all_distances(current->main_character)));
    free((*player_seen_map(current->main_character)));
    player_delete(current->main_character);
}
