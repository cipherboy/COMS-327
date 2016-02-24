
/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy implementation file
**/

#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "map_enemy.h"
#include "utils.h"

void map_enemy_init(map* current)
{
    current->enemies = malloc(sizeof(enemy) * current->enemy_count);
    for (int i = 0; i < current->enemy_count; i++) {
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

        current->enemies[i].pos_x = pos_x;
        current->enemies[i].pos_y = pos_y;
        current->enemies[i].attributes = attributes;
        current->enemies[i].speed = speed;
        current->enemies[i].representation = representation;
        current->enemies[i].is_alive = true;
    }
}

void map_enemy_render(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            current->characters_location[y][x] = NULL;
            current->characters_layer[y][x] = ' ';
        }
    }
    current->characters_location[current->main_character.pos_y][current->main_character.pos_x] = (void *) &current->main_character;
    current->characters_layer[current->main_character.pos_y][current->main_character.pos_x] = '@';

    for (int i = 0; i < current->enemy_count; i++) {
        if (current->enemies[i].is_alive) {

            if (current->characters_location[current->enemies[i].pos_y][current->enemies[i].pos_x] != NULL) {
                printf("Uncaught collision!");
            }
            current->characters_location[current->enemies[i].pos_y][current->enemies[i].pos_x] = (void *) &current->enemies[i];
            current->characters_layer[current->enemies[i].pos_y][current->enemies[i].pos_x] = current->enemies[i].representation;
        }
    }
}

void map_enemy_update_last_seen(map* current, int enemy_loc)
{
    int telepathic = current->enemies[enemy_loc].attributes & ENEMY_ATTRIBUTE_TELEPATHY;
    if (telepathic) {
        current->enemies[enemy_loc].has_seen_main_character = true;
        current->enemies[enemy_loc].main_character_last_x = current->main_character.pos_x;
        current->enemies[enemy_loc].main_character_last_y = current->main_character.pos_y;
    } else {
        int main_character_room_number = map_rooms_find_contains_point(current, current->main_character.pos_x, current->main_character.pos_y);
        int enemy_room_number = map_rooms_find_contains_point(current, current->enemies[enemy_loc].pos_x, current->enemies[enemy_loc].pos_y);

        if (main_character_room_number == enemy_room_number && enemy_room_number != -1) {
            current->enemies[enemy_loc].has_seen_main_character = true;
            current->enemies[enemy_loc].main_character_last_x = current->main_character.pos_x;
            current->enemies[enemy_loc].main_character_last_y = current->main_character.pos_y;
        } else {
            // Not in the same room, now the fun begins.
            bool all_zeroes = true;

            int left_x;
            int left_y;
            int right_x;
            int right_y;

            if (current->main_character.pos_x < current->enemies[enemy_loc].pos_x) {
                left_x = current->main_character.pos_x;
                left_y = current->main_character.pos_y;

                right_x = current->enemies[enemy_loc].pos_x;
                right_y = current->enemies[enemy_loc].pos_y;
            } else {
                left_x = current->enemies[enemy_loc].pos_x;
                left_y = current->enemies[enemy_loc].pos_y;

                right_x = current->main_character.pos_x;
                right_y = current->main_character.pos_y;
            }

            double slope = ((double) (left_y - right_y)) / ((double) (left_x - right_x));

            for (int dx = 0; left_x + dx <= right_x; dx += 1) {
                int new_pos_x = left_x + dx;
                int new_pos_y = slope*new_pos_x;

                if (new_pos_x > 0 && new_pos_x < current->cols && new_pos_y > 0 && new_pos_y < current->rows) {
                    if (current->rock_hardness[new_pos_y][new_pos_x] != 0) {
                        all_zeroes = false;
                        break;
                    }
                } else if (!(new_pos_x > 0 && new_pos_x < current->cols && new_pos_y > 0 && new_pos_y < current->rows)) {
                    all_zeroes = false;
                    break;
                }
            }

            if (all_zeroes) {
                current->enemies[enemy_loc].has_seen_main_character = true;
                current->enemies[enemy_loc].main_character_last_x = current->main_character.pos_x;
                current->enemies[enemy_loc].main_character_last_y = current->main_character.pos_y;
            }
        }
    }
}

void map_enemy_move(map* current, int enemy_loc)
{
    func_table_element func_table[] = {
        map_enemy_move_not_intelligent_not_telepathic,
        map_enemy_move_intelligent_not_telepathic,
        map_enemy_move_not_intelligent_telepathic,
        map_enemy_move_intelligent_telepathic,
        map_enemy_move_not_intelligent_not_telepathic,
        map_enemy_move_intelligent_not_telepathic,
        map_enemy_move_not_intelligent_telepathic,
        map_enemy_move_intelligent_telepathic,
    };

    if (!current->enemies[enemy_loc].is_alive) {
        return;
    }

    int random = current->enemies[enemy_loc].attributes & ENEMY_ATTRIBUTE_ERRATIC;
    if (random) {
        map_enemy_move_erratic(current, enemy_loc);
    } else {
        (* func_table[current->enemies[enemy_loc].attributes]) (current, enemy_loc);
    }

}

void map_enemy_deinit(map* current)
{
    printf("Enemies freed~");
    free(current->enemies);
}
