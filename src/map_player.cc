/**
 * Copyright 2016 Alexander Scheel
 *
 * Map player implementation file
**/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "player.h"
#include "character.h"
#include "map.h"
#include "utils.h"
#include "node_distances.h"
#include "binheap.h"

void map_player_init(map_c* current, player_t* player)
{
    current->main_character = player;
}

void map_player_distances(map_c* current)
{
    map_player_distances_blank(current);
    map_player_distances_bounded(current);
    map_player_distances_unbounded(current);
}

void map_player_distances_blank(map_c* current)
{
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            (current->main_character->player_distances[y][x]) = 255;
            (current->main_character->all_distances[y][x]) = 255;
        }
    }
}

void map_player_distances_bounded(map_c* current)
{
    binheap_t queue;
    binheap_init(&queue, distances_distance, NULL);

    distances** objects = (distances **) malloc(sizeof(distances*) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = (binheap_node_t ***) malloc(sizeof(binheap_node_t**) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = (distances *) malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = (binheap_node_t **) malloc(sizeof(binheap_node_t*) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != current->main_character->pos_y|| x != current->main_character->pos_x ) {
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
                (current->main_character->player_distances[y][x]) = objects[y][x].distance;
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

void map_player_distances_unbounded(map_c* current)
{
    binheap_t queue;
    binheap_init(&queue, distances_distance, NULL);

    distances** objects = (distances **) malloc(sizeof(distances *) * current->rows * current->cols);
    binheap_node_t*** bh_ptr = (binheap_node_t ***) malloc(sizeof(binheap_node_t**) * current->rows * current->cols);

    // No need to add character: he starts in a room
    for (int y = 0; y < current->rows; y++) {
        objects[y] = (distances *) malloc(sizeof(distances) * current->cols);
        bh_ptr[y] = (binheap_node_t **) malloc(sizeof(binheap_node_t*) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            objects[y][x].x = x;
            objects[y][x].y = y;
            if (y != current->main_character->pos_y|| x != current->main_character->pos_x ) {
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
            (current->main_character->all_distances[y][x]) = objects[y][x].distance;
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

void map_player_move(map_c* current, int dx, int dy)
{
    int pos_x = current->main_character->pos_x;
    int pos_y = current->main_character->pos_y;

    map_enemy_render(current);

    pos_x = current->main_character->pos_x + dx;
    pos_y = current->main_character->pos_y + dy;

    while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        pos_x = current->main_character->pos_x + dx;
        pos_y = current->main_character->pos_y + dy;
    }

    if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
        if (current->rock_hardness[pos_y][pos_x] > 85) {
            current->rock_hardness[pos_y][pos_x] -= 85;
        } else {
            current->rock_hardness[pos_y][pos_x] = 0;
            current->hallways_layer[pos_y][pos_x] = '#';

            if ((current->main_character->pos_x!= pos_x || current->main_character->pos_y!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                int player_damage = current->main_character->attack_damage.roll();

                if (current->display_message != NULL) {
                    free(current->display_message);
                }

                current->display_message = (char *) malloc(sizeof(char) * 75);
                snprintf(current->display_message, 74, "Attacked monster at %i, %i: %c for %i HP.", pos_x, pos_y, current->characters_location[pos_y][pos_x]->representation, player_damage);
                current->display_turn = 0;

                current->characters_location[pos_y][pos_x]->hp -= player_damage;

                if (current->characters_location[pos_y][pos_x]->hp <= 0) {
                    current->characters_location[pos_y][pos_x]->is_alive = false;
                }

                pos_x = current->main_character->pos_x;
                pos_y = current->main_character->pos_y;
            }

            current->main_character->pos_x = pos_x;
            current->main_character->pos_y = pos_y;
        }

        map_player_distances(current);
    } else if (current->rock_hardness[pos_y][pos_x] == 0) {
        current->hallways_layer[pos_y][pos_x] = '#';

        if ((current->main_character->pos_x!= pos_x || current->main_character->pos_y!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
            int player_damage = current->main_character->attack_damage.roll();

            for (int i = 0; i < 12; i++) {
                if (current->main_character->equipment[i] != NULL) {
                    player_damage += current->main_character->equipment[i]->damage.roll();
                }
            }

            if (current->display_message != NULL) {
                free(current->display_message);
            }

            current->display_message = (char *) malloc(sizeof(char) * 75);
            snprintf(current->display_message, 74, "Attacked monster at %i, %i: %c for %i HP.", pos_x, pos_y, current->characters_location[pos_y][pos_x]->representation, player_damage);
            current->display_turn = 0;

            current->characters_location[pos_y][pos_x]->hp -= player_damage;

            if (current->characters_location[pos_y][pos_x]->hp <= 0) {
                current->characters_location[pos_y][pos_x]->is_alive = false;
            }

            pos_x = current->main_character->pos_x;
            pos_y = current->main_character->pos_y;
        }

        current->main_character->pos_x = pos_x;
        current->main_character->pos_y = pos_y;
    }

    if (current->main_character->inventory->stack_size < 10 && current->objects_location[current->main_character->pos_y][current->main_character->pos_x] != NULL) {
        object_t* obj = current->objects_location[current->main_character->pos_y][current->main_character->pos_x];

        if (obj->display == true) {
            while (current->main_character->inventory->stack_size < 10) {
                if (obj->representation == '&') {
                    object_t* tmp = obj->pick_from_top_of_stack();
                    if (tmp->representation != '&') {
                        current->main_character->inventory->add_to_stack(tmp);
                    }
                } else {
                    current->main_character->inventory->add_to_stack(obj);
                    current->objects_location[current->main_character->pos_y][current->main_character->pos_x] = NULL;
                    break;
                }
            }
        }
    }
}

void map_player_deinit(map_c* current)
{
    delete current->main_character;
}
