/**
 * Copyright 2016 Alexander Scheel
 *
 * Object iimplementation file
**/

#include "dice.h"
#include "object.h"
#include "ncurses.h"
#include <cstring>
#include <string>

using namespace std;

object_t* object_f::make_widget()
{
    object_t* object = new object_t();

    object->name = this->name;
    object->description = this->description;
    object->type = this->type;

    int color_i = 0;

    char * token = strtok((char *) this->color.c_str(), " ");

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

    object->color = color_i;

    object->hit = this->hit.roll();
    object->damage = this->damage;
    object->dodge = this->dodge.roll();
    object->defense = this->defense.roll();
    object->weight = this->weight.roll();
    object->speed = this->speed.roll();
    object->attr = this->attr.roll();
    object->value = this->value.roll();

    return object;
}
