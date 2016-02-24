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
    int room = rand() % current->room_count;
    current->main_character.speed = 10;

    int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
    int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

    current->main_character.pos_x = pos_x;
    current->main_character.pos_y = pos_y;

    current->main_character.is_alive = true;

    current->characters_layer[pos_y][pos_x] = '@';
}

void map_player_distances(map* current)
{
    map_player_distances_blank(current);
    map_player_distances_bounded(current);
    map_player_distances_unbounded(current);
}

void map_player_distances_blank(map* current)
{
    current->main_character.player_distances = malloc(sizeof(char) * current->rows * current->cols);
    current->main_character.all_distances = malloc(sizeof(char) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        current->main_character.player_distances[y] = malloc(sizeof(char) * current->cols);
        current->main_character.all_distances[y] = malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            current->main_character.player_distances[y][x] = 255;
            current->main_character.all_distances[y][x] = 255;
        }
    }
}

void map_player_distances_bounded(map* current)
{
    binheap_t queue;
    binheap_init(&queue, distances_distance, NULL);

    distances** objects = malloc(sizeof(distances) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = malloc(sizeof(binheap_node_t) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = malloc(sizeof(binheap_node_t) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != current->main_character.pos_y || x != current->main_character.pos_x) {
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
                current->main_character.player_distances[y][x] = objects[y][x].distance;
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

    distances** objects = malloc(sizeof(distances) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = malloc(sizeof(binheap_node_t) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = malloc(sizeof(binheap_node_t) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != current->main_character.pos_y || x != current->main_character.pos_x) {
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
            current->main_character.all_distances[y][x] = objects[y][x].distance;
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

void map_player_move(map* current)
{
    int pos_x = current->main_character.pos_x;
    int pos_y = current->main_character.pos_y;

    map_enemy_render(current);

    for (int ly = -1; ly <= 1; ly++) {
        for (int lx = -1; lx <= 1; lx++) {
            if (lx == ly && lx == 0) {
                continue;
            }

            if (current->characters_location[pos_y+lx][pos_x+ly] != NULL) {
                pos_x = current->main_character.pos_x + lx;
                pos_y = current->main_character.pos_y + ly;
                lx = 10000;
                ly = 10000;
                break;
            }
        }
    }

    if (pos_x != current->main_character.pos_x && pos_y != current->main_character.pos_y && current->rock_hardness[pos_y][pos_x] <= 85) {
        // New location!
        current->rock_hardness[pos_y][pos_x] = 0;
        current->hallways_layer[pos_y][pos_x] = '#';

        if (current->characters_location[pos_y][pos_x] != NULL) {
            ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
        }

        current->main_character.pos_x = pos_x;
        current->main_character.pos_y = pos_y;
        return;
    }

    pos_x = current->main_character.pos_x;
    pos_y = current->main_character.pos_y;

    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;

    while (dy == 0 && dx == 0) {
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    }

    pos_x = current->main_character.pos_x + dx;
    pos_y = current->main_character.pos_y + dy;

    while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        pos_x = current->main_character.pos_x + dx;
        pos_y = current->main_character.pos_y + dy;
    }

    if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
        if (current->rock_hardness[pos_y][pos_x] > 85) {
            current->rock_hardness[pos_y][pos_x] -= 85;
        } else {
            current->rock_hardness[pos_y][pos_x] = 0;
            current->hallways_layer[pos_y][pos_x] = '#';

            if ((current->main_character.pos_x != pos_x || current->main_character.pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
            }

            current->main_character.pos_x = pos_x;
            current->main_character.pos_y = pos_y;
        }

        map_player_deinit(current);
        map_player_distances(current);
    } else if (current->rock_hardness[pos_y][pos_x] == 0) {
        current->hallways_layer[pos_y][pos_x] = '#';

        if ((current->main_character.pos_x != pos_x || current->main_character.pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
            ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
        }

        current->main_character.pos_x = pos_x;
        current->main_character.pos_y = pos_y;
    }
}

void map_player_deinit(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        free(current->main_character.player_distances[y]);
        free(current->main_character.all_distances[y]);
    }
    free(current->main_character.player_distances);
    free(current->main_character.all_distances);
}
