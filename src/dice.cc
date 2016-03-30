/**
 * Copyright 2016 Alexander Scheel
 *
 * Dice implementation file
**/

#include "dice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

dice_t::dice_t()
{
    this->base = rand() % 10;
    this->dice = rand() % 3;
    this->sides = rand() % 30;
}

dice_t::dice_t(int base, int dice, int sides)
{
    this->base = base;
    this->dice = dice;
    this->sides = sides;
}

bool dice_t::parse(char* line)
{
    int max_line_length = 1000;

    int which = 0;
    char* base = NULL;
    char* dice = NULL;
    char* sides = NULL;

    while (*(line++) != '\0' && max_line_length--) {
        int diff = (*line) - '0';
        if (diff >= 0 && diff <= 9) {
            switch (which) {
            case 0:
                base = line;
                which++;
                break;
            case 1:
                dice = line;
                which++;
                break;
            case 2:
                sides = line;
                which++;
                break;
            }
        }
    }

    if (which != 3) {
        return false;
    }

    this->base = atoi(base);
    this->dice = atoi(dice);
    this->sides = atoi(sides);

    return true;
}

int dice_t::roll()
{
    int result = this->base;
    int i = 0;
    for (i = 0; i < this->dice; i++) {
        result += (rand() % this->sides) + 1;
    }

    return result;
}

string dice_t::print()
{
    std::ostringstream output;

    output << this->base;
    output << " + ";
    output << this->dice;
    output << "d";
    output << this->sides;
    return output.str();
}
