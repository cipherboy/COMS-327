
/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy implementation file
**/

#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "map_enemy.h"

void map_enemy_init(map* current)
{
    current->enemies = malloc(sizeof(enemy) * current->enemy_count);
    for (int i = 0; i < current->enemy_count; i++) {
        enemy new;
        int room = rand() % current->room_count;
        int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
        int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

        bool has_conflict = false;
        if (pos_x == current->main_character.pos_x && pos_y == current->main_character.pos_y) {
            has_conflict = true;
        } else {
            for (int j = 0; j < i; j++) {
                if (pos_x == current->enemies[j].pos_x && pos_y == current->enemies[j].pos_y) {
                    has_conflict = true;
                    break;
                }
            }
        }

        while (has_conflict) {
            room = rand() % current->room_count;
            pos_x = current->rooms[room].pos_x + rand() % (current->rooms[room].width);
            pos_y = current->rooms[room].pos_y + rand() % (current->rooms[room].height);

            has_conflict = false;
            if (pos_x == current->main_character.pos_x && pos_y == current->main_character.pos_y) {
                has_conflict = true;
            } else {
                for (int j = 0; j < i; j++) {
                    if (pos_x == current->enemies[j].pos_x && pos_y == current->enemies[j].pos_y) {
                        has_conflict = true;
                        break;
                    }
                }
            }
        }

        int attributes = rand() % ENEMY_ATTRIBUTE_MAX;
        int speed = rand() % 15 + 5;
        char representation = '?';

        // Yeah, yeah...
        if (attributes < 10) {
            representation = '0' + attributes;
        } else {
            representation = 'a' + attributes-10;
        }

        new.pos_x = pos_x;
        new.pos_y = pos_y;
        new.attributes = attributes;
        new.speed = speed;
        new.representation = representation;

        current->enemies[i] = new;
    }
}

void map_enemy_render(map* current)
{
    for (int i = 0; i < current->enemy_count; i++) {
        current->characters_layer[current->enemies[i].pos_y][current->enemies[i].pos_x] = current->enemies[i].representation;
    }
}

void map_enemy_deinit(map* current)
{
    printf("Enemies freed~");
    free(current->enemies);
}
