
/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy implementation file
**/

#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "character.h"
#include "player.h"
#include "enemy.h"
#include "map_enemy.h"
#include "utils.h"

void map_enemy_init(map_c* current)
{
    current->enemies = (enemy_t**) malloc(sizeof(enemy_t*) * current->enemy_count);
    for (int i = 0; i < current->enemy_count; i++) {
        int factory = rand() % current->monster_factory_count;
        current->enemies[i] = current->monster_factories[factory]->make_widget();
    }

    for (int i = 0; i < current->enemy_count; i++) {
        int room = rand() % current->room_count;
        int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
        int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

        bool has_conflict = false;
        if (pos_x == current->main_character->pos_x&& pos_y == current->main_character->pos_y ) {
            has_conflict = true;
        } else {
            for (int j = 0; j < i; j++) {
                if (pos_x == current->enemies[j]->pos_x&& pos_y == current->enemies[j]->pos_y ) {
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
            if (pos_x == current->main_character->pos_x&& pos_y == current->main_character->pos_y ) {
                has_conflict = true;
            } else {
                for (int j = 0; j < i; j++) {
                    if (pos_x == current->enemies[j]->pos_x&& pos_y == current->enemies[j]->pos_y ) {
                        has_conflict = true;
                        break;
                    }
                }
            }
        }

        current->enemies[i]->pos_x = pos_x;
        current->enemies[i]->pos_y = pos_y;
        map_enemy_update_last_seen(current, i);
    }
}

void map_enemy_render(map_c* current)
{
    // Turned into no-op.
}

void map_enemy_update_last_seen(map_c* current, int enemy_loc)
{
    int telepathic = current->enemies[enemy_loc]->attributes & ENEMY_ATTRIBUTE_TELEPATHY;
    if (telepathic) {
        current->enemies[enemy_loc]->has_seen_main_character = true;
        current->enemies[enemy_loc]->main_character_last_x = current->main_character->pos_x;
        current->enemies[enemy_loc]->main_character_last_y = current->main_character->pos_y;
        return;
    }
    int main_character_room_number = map_rooms_find_contains_point(current, current->main_character->pos_x, current->main_character->pos_y);
    int enemy_room_number = map_rooms_find_contains_point(current, current->enemies[enemy_loc]->pos_x, current->enemies[enemy_loc]->pos_y);

    if (main_character_room_number == enemy_room_number && enemy_room_number != -1) {
        current->enemies[enemy_loc]->has_seen_main_character = true;
        current->enemies[enemy_loc]->main_character_last_x = current->main_character->pos_x;
        current->enemies[enemy_loc]->main_character_last_y = current->main_character->pos_y;
        return;
    }

    // Not in the same room, now the fun begins.
    bool all_zeroes = true;

    int left_x;
    int left_y;
    int right_x;
    int right_y;

    if (current->main_character->pos_x < current->enemies[enemy_loc]->pos_x) {
        left_x = current->main_character->pos_x;
        left_y = current->main_character->pos_y;

        right_x = current->enemies[enemy_loc]->pos_x;
        right_y = current->enemies[enemy_loc]->pos_y;
    } else {
        left_x = current->enemies[enemy_loc]->pos_x;
        left_y = current->enemies[enemy_loc]->pos_y;

        right_x = current->main_character->pos_x;
        right_y = current->main_character->pos_y;
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
        current->enemies[enemy_loc]->has_seen_main_character = true;
        current->enemies[enemy_loc]->main_character_last_x = current->main_character->pos_x;
        current->enemies[enemy_loc]->main_character_last_y = current->main_character->pos_y;
    }
}

void map_enemy_move(map_c* current, int enemy_loc)
{
    if (!current->enemies[enemy_loc]->is_alive || current->enemies[enemy_loc]->hp <= 0) {
        return;
    }

    // Default: if player is right next to the enemy, attack!
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ny = current->enemies[enemy_loc]->pos_y + dy;
            int nx = current->enemies[enemy_loc]->pos_x + dx;

            if ((dx != 0 || dy != 0) && ny == current->main_character->pos_y && nx == current->main_character->pos_x) {
                int monster_damage = current->enemies[enemy_loc]->attack_damage.roll();

                if (current->display_message != NULL) {
                    free(current->display_message);
                }

                current->display_message = (char *) malloc(sizeof(char) * 75);
                snprintf(current->display_message, 74, "Attacked by monster at %i, %i: %c for %i HP.", current->enemies[enemy_loc]->pos_x, current->enemies[enemy_loc]->pos_y, current->enemies[enemy_loc]->representation, monster_damage);
                current->display_turn = 0;

                current->main_character->hp -= monster_damage;

                return;
            }
        }
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

    int random = current->enemies[enemy_loc]->attributes & ENEMY_ATTRIBUTE_ERRATIC;
    if (random) {
        map_enemy_move_erratic(current, enemy_loc);
    } else {
        (* func_table[current->enemies[enemy_loc]->attributes]) (current, enemy_loc);
    }

}

void map_enemy_deinit(map_c* current)
{
    for (int i = 0; i < current->enemy_count; i++) {
        if (current->enemies[i] != NULL) {
            delete current->enemies[i];
        }
    }
    free(current->enemies);
}
