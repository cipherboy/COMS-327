
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
    current->enemies = malloc(sizeof(enemy*) * current->enemy_count);
    for (int i = 0; i < current->enemy_count; i++) {
        current->enemies[i] = enemy_new();
    }

    for (int i = 0; i < current->enemy_count; i++) {
        int room = rand() % current->room_count;
        int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
        int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

        bool has_conflict = false;
        if (pos_x == (*character_pos_x(current->main_character) )&& pos_y == (*character_pos_y(current->main_character)) ) {
            has_conflict = true;
        } else {
            for (int j = 0; j < i; j++) {
                if (pos_x == (*character_pos_x(current->enemies[j]) )&& pos_y == (*character_pos_y(current->enemies[j])) ) {
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
            if (pos_x == (*character_pos_x(current->main_character) )&& pos_y == (*character_pos_y(current->main_character)) ) {
                has_conflict = true;
            } else {
                for (int j = 0; j < i; j++) {
                    if (pos_x == (*character_pos_x(current->enemies[j]) )&& pos_y == (*character_pos_y(current->enemies[j])) ) {
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

        (*character_pos_x(current->enemies[i])) = pos_x;
        (*character_pos_y(current->enemies[i])) = pos_y;

        (*enemy_attributes(current->enemies[i])) = attributes;
        (*character_speed(current->enemies[i])) = speed;
        (*character_representation(current->enemies[i])) = representation;
        (*character_is_alive(current->enemies[i])) = true;
        (*enemy_has_seen_main_character(current->enemies[i])) = false;
        (*enemy_main_character_last_x(current->enemies[i])) = 2;
        (*enemy_main_character_last_y(current->enemies[i])) = 2;
        map_enemy_update_last_seen(current, i);
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

    if ((*character_is_alive(current->main_character)) ) {
        current->characters_location[(*character_pos_y(current->main_character))][(*character_pos_x(current->main_character))] = (void *) &current->main_character;
        current->characters_layer[(*character_pos_y(current->main_character))][(*character_pos_x(current->main_character))] = (*character_representation(current->main_character));
    }

    for (int dy = -3; dy <= 3; dy++) {
        for (int dx = -3; dx <= 3; dx++) {
            int y = (*character_pos_y(current->main_character)) + dy;
            int x = (*character_pos_x(current->main_character)) + dx;
            if (current->rooms_layer[y][x] != ' ') {
                ((*player_seen_map(current->main_character))[y][x]) = current->rooms_layer[y][x];
            } else if (current->hallways_layer[y][x] != ' ') {
                ((*player_seen_map(current->main_character))[y][x]) = current->hallways_layer[y][x];
            }
        }
    }

    for (int i = 0; i < current->enemy_count; i++) {
        if ((*character_is_alive(current->enemies[i])) ) {

            if (current->characters_location[(*character_pos_y(current->enemies[i]))][(*character_pos_x(current->enemies[i]))] != NULL) {
                printf("Uncaught collision!");
            }
            current->characters_location[(*character_pos_y(current->enemies[i]))][(*character_pos_x(current->enemies[i]))] = (void *) &current->enemies[i];
            current->characters_layer[(*character_pos_y(current->enemies[i]))][(*character_pos_x(current->enemies[i]))] =  (*character_representation(current->enemies[i]));
        }
    }
}

void map_enemy_update_last_seen(map* current, int enemy_loc)
{
    int telepathic = (*enemy_attributes(current->enemies[enemy_loc]) )& ENEMY_ATTRIBUTE_TELEPATHY;
    if (telepathic) {
        (*enemy_has_seen_main_character(current->enemies[enemy_loc])) = true;
        (*enemy_main_character_last_x(current->enemies[enemy_loc])) = (*character_pos_x(current->main_character));
        (*enemy_main_character_last_y(current->enemies[enemy_loc])) = (*character_pos_y(current->main_character));
        return;
    }
    int main_character_room_number = map_rooms_find_contains_point(current, (*character_pos_x(current->main_character)), (*character_pos_y(current->main_character)));
    int enemy_room_number = map_rooms_find_contains_point(current, (*character_pos_x(current->enemies[enemy_loc])), (*character_pos_y(current->enemies[enemy_loc])));

    if (main_character_room_number == enemy_room_number && enemy_room_number != -1) {
        (*enemy_has_seen_main_character(current->enemies[enemy_loc])) = true;
        (*enemy_main_character_last_x(current->enemies[enemy_loc])) = (*character_pos_x(current->main_character));
        (*enemy_main_character_last_y(current->enemies[enemy_loc])) = (*character_pos_y(current->main_character));
        return;
    }

    // Not in the same room, now the fun begins.
    bool all_zeroes = true;

    int left_x;
    int left_y;
    int right_x;
    int right_y;

    if ((*character_pos_x(current->main_character)) < (*character_pos_x(current->enemies[enemy_loc]))) {
        left_x = (*character_pos_x(current->main_character));
        left_y = (*character_pos_y(current->main_character));

        right_x = (*character_pos_x(current->enemies[enemy_loc]));
        right_y = (*character_pos_y(current->enemies[enemy_loc]));
    } else {
        left_x = (*character_pos_x(current->enemies[enemy_loc]));
        left_y = (*character_pos_y(current->enemies[enemy_loc]));

        right_x = (*character_pos_x(current->main_character));
        right_y = (*character_pos_y(current->main_character));
    }

    if (left_x - right_x != 0) {
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
    } else {
        int top_y = 0;
        int bottom_y = 0;

        if (left_y < right_y) {
            bottom_y = left_y;
            top_y = right_y;
        } else {
            bottom_y = right_y;
            top_y = left_y;
        }

        for (int dy = 0; bottom_y + dy <= top_y; dy++) {
            int new_pos_y = bottom_y + dy;

            if (new_pos_y > 0 && new_pos_y < current->rows) {
                if (current->rock_hardness[new_pos_y][left_x] != 0) {
                    all_zeroes = false;
                    break;
                }
            } else if (!(new_pos_y > 0 && new_pos_y < current->rows)) {
                all_zeroes = false;
                break;
            }
        }
    }

    if (all_zeroes) {
        (*enemy_has_seen_main_character(current->enemies[enemy_loc])) = true;
        (*enemy_main_character_last_x(current->enemies[enemy_loc])) = (*character_pos_x(current->main_character));
        (*enemy_main_character_last_y(current->enemies[enemy_loc])) = (*character_pos_y(current->main_character));
    }
}

void map_enemy_move(map* current, int enemy_loc)
{
    if (!(*character_is_alive(current->enemies[enemy_loc])) ) {
        return;
    }

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

    int random = (*enemy_attributes(current->enemies[enemy_loc]) )& ENEMY_ATTRIBUTE_ERRATIC;
    if (random) {
        map_enemy_move_erratic(current, enemy_loc);
    } else {
        (* func_table[*enemy_attributes(current->enemies[enemy_loc])]) (current, enemy_loc);
    }

}

void map_enemy_deinit(map* current)
{
    for (int i = 0; i < current->enemy_count; i++) {
        if (current->enemies[i] != NULL) {
            enemy_delete(current->enemies[i]);
        }
    }
    free(current->enemies);
}
