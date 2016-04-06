/**
 * Copyright 2016 Alexander Scheel
 *
 * Character implementation file
**/


#include <stdio.h>
#include <ncurses.h>
#include "stdlib.h"
#include <string.h>
#include "character.h"
#include "enemy.h"
#include "map.h"

character_t::character_t(map_c* current)
{
    this->current = current;

    int room = rand() % current->room_count;
    this->speed = 10;

    int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
    int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

    this->pos_x = pos_x;
    this->pos_y = pos_y;

    this->is_alive = true;
    this->representation = '@';

    this->color = 0;
}

character_f::character_f(map_c* current)
{
    this->current = current;
    this->representation = '`';
}

enemy_t* character_f::make_widget()
{
    enemy_t* enemy = new enemy_t(this->current);
    enemy->is_alive = true;
    enemy->has_seen_main_character = false;
    enemy->main_character_last_x = 2;
    enemy->main_character_last_y = 2;
    enemy->hp = this->hp.roll();
    enemy->speed = this->speed_dice.roll();
    enemy->attack_damage = this->attack_damage;
    enemy->representation = this->representation;

    enemy->name = this->name;
    enemy->description = this->description;

    int attr_i = 0;

    char* token;
    token = strtok((char *) this->abilities.c_str(), " ");

    while (token != NULL) {
        if (strcmp(token, "SMART") == 0) {
            attr_i += ENEMY_ATTRIBUTE_INTELLIGENCE;
        } else if (strcmp(token,"TELE") == 0) {
            attr_i += ENEMY_ATTRIBUTE_TELEPATHY;
        } else if (strcmp(token, "TUNNEL") == 0) {
            attr_i += ENEMY_ATTRIBUTE_TUNNELING;
        } else if (strcmp(token, "ERRATIC") == 0) {
            attr_i += ENEMY_ATTRIBUTE_ERRATIC;
        } else if (strcmp(token, "PASS") == 0) {
            attr_i += 0;
        }

        token = strtok(NULL, " ");
    }

    enemy->attributes = attr_i;

    int color_i = 0;

    token = strtok((char *) this->color.c_str(), " ");

    while (token != NULL) {
        if (strcmp(token, "BLACK") == 0) {
            color_i = COLOR_BLACK+1;
        } else if (strcmp(token, "RED") == 0) {
            color_i = COLOR_RED+1;
        } else if (strcmp(token, "GREEN") == 0) {
            color_i = COLOR_GREEN+1;
        } else if (strcmp(token, "YELLOW") == 0) {
            color_i = COLOR_YELLOW+1;
        } else if (strcmp(token, "MAGENTA") == 0) {
            color_i = COLOR_MAGENTA+1;
        } else if (strcmp(token, "BLUE") == 0) {
            color_i = COLOR_BLUE+1;
        } else if (strcmp(token, "CYAN") == 0) {
            color_i = COLOR_CYAN+1;
        } else if (strcmp(token, "WHITE") == 0) {
            color_i = COLOR_WHITE+1;
        }

        token = strtok(NULL, " ");
    }

    enemy->color = color_i;

    return enemy;
}
