/**
 * Copyright 2016 Alexander Scheel
 *
 * Enemy header file
**/

#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_ATTRIBUTE_INTELLIGENCE 0x00000001
#define ENEMY_ATTRIBUTE_TELEPATHY 0x00000002
#define ENEMY_ATTRIBUTE_TUNNELING 0x00000004
#define ENEMY_ATTRIBUTE_ERRATIC 0x00000008
#define ENEMY_ATTRIBUTE_5 0x00000010
#define ENEMY_ATTRIBUTE_6 0x00000020
#define ENEMY_ATTRIBUTE_7 0x00000040
#define ENEMY_ATTRIBUTE_8 0x00000080
#define ENEMY_ATTRIBUTE_MAX 16

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int pos_x;
    int pos_y;

    bool is_alive;

    int attributes;
    char representation;

    int speed;

    bool has_seen_main_character;
} enemy;

#endif
