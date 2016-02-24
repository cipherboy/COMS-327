/**
 * Copyright 2016 Alexander Scheel
 *
 * Map layers header file
**/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "map.h"
#include "binheap.h"
#include "node_moveable.h"

void map_main(map* current)
{
    binheap_t queue;
    binheap_init(&queue, moveables_moveable, NULL);

    moveable* objects = malloc(sizeof(moveable) * (current->enemy_count+1));
    binheap_node_t** bh_ptr = malloc(sizeof(binheap_node_t) * (current->enemy_count+1));

    objects[0].order = 0;
    objects[0].is_player = true;
    objects[0].speed = current->main_character.speed;
    objects[0].next_turn = 0;
    bh_ptr[0] = binheap_insert(&queue, &objects[0]);

    for (int i = 0; i < current->enemy_count; i++) {
        objects[i+1].order = i+1;
        objects[i+1].is_player = false;
        objects[i+1].speed = current->enemies[i].speed;
        objects[i+1].next_turn = 0;
        bh_ptr[i+1] = binheap_insert(&queue, &objects[i+1]);
    }
    map_enemy_render(current);
    map_print(current);
    usleep(10000);

    moveable* c;

    while (true) {
        c = (moveable*) binheap_remove_min(&queue);

        objects[c->order].next_turn += 100/c->speed;

        if (!c->is_player && current->enemies[c->order-1].is_alive) {
            map_enemy_move(current, c->order-1);
            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        } else if (c->is_player) {
            map_player_move(current);
            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        }

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

        for (int y = 0; y < current->rows; y++) {
            for (int x = 0; x < current->cols; x++) {
                current->characters_layer[y][x] = ' ';
            }
        }

        if (!current->main_character.is_alive) {
            printf("Main character died! :(\n");
            break;
        } else {
            bool has_enemies = false;;
            for (int i = 0; i < current->enemy_count; i++) {
                if (current->enemies[i].is_alive) {
                    has_enemies = true;
                }
            }
            if (!has_enemies) {
                printf("Main character won!\n");
                break;
            }
        }

        map_enemy_render(current);
        map_print(current);
        usleep(10000);
    }


    binheap_delete(&queue);

    free(objects);
    free(bh_ptr);

}
