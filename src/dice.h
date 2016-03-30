/**
 * Copyright 2016 Alexander Scheel
 *
 * Dice header file
**/

#pragma once
#ifndef DICE_H
#define DICE_H

class dice_t {
public:
    int base;
    int dice;
    int sides;

    dice_t();
    dice_t(int base, int dice, int sides);

    bool parse(char* line);
    int  roll();
};

#endif
