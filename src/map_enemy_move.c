/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy move header file
**/

#include <stdlib.h>
#include <stdio.h>

#include "map.h"

void map_enemy_move_random(map* current, int enemy)
{
    if (rand() % 2 == 0) {

        int dx = rand() % 3 - 1;
        int dy = rand() % 3 - 1;

        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        int pos_x = current->enemies[enemy].pos_x + dx;
        int pos_y = current->enemies[enemy].pos_y + dy;

        while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
            while (dy == 0 && dx == 0) {
                dx = rand() % 3 - 1;
                dy = rand() % 3 - 1;
            }

            pos_x = current->enemies[enemy].pos_x + dx;
            pos_y = current->enemies[enemy].pos_y + dy;
        }

        int tunneling = current->enemies[enemy].attributes & ENEMY_ATTRIBUTE_TUNNELING;
        if (tunneling) {
            if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
                if (current->rock_hardness[pos_y][pos_x] > 85) {
                    current->rock_hardness[pos_y][pos_x] -= 85;
                } else {
                    current->rock_hardness[pos_y][pos_x] = 0;
                    current->hallways_layer[pos_y][pos_x] = '#';
                    current->enemies[enemy].pos_x = pos_x;
                    current->enemies[enemy].pos_y = pos_y;
                }

                map_player_distances(current);
            } else if (current->rock_hardness[pos_y][pos_x] == 0) {
                current->hallways_layer[pos_y][pos_x] = '#';
                current->enemies[enemy].pos_x = pos_x;
                current->enemies[enemy].pos_y = pos_y;
            }
        } else {
            int tries = 0;
            while (current->rock_hardness[pos_y][pos_x] != 0 && tries <= 100) {
                while (dy == 0 && dx == 0) {
                    dx = rand() % 3 - 1;
                    dy = rand() % 3 - 1;
                    tries += 1;
                }

                pos_x = current->enemies[enemy].pos_x + dx;
                pos_y = current->enemies[enemy].pos_y + dy;

                tries += 1;
            }

            if (tries >= 100) {
                printf("move_random: Unable to find suitable path!\n");
                return;
            }

            current->enemies[enemy].pos_x = pos_x;
            current->enemies[enemy].pos_y = pos_y;
        }
    } else {
        current->enemies[enemy].attributes -= ENEMY_ATTRIBUTE_ERRATIC;
        map_enemy_move(current, enemy);
        current->enemies[enemy].attributes += ENEMY_ATTRIBUTE_ERRATIC;
    }
}
