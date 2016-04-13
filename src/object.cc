/**
 * Copyright 2016 Alexander Scheel
 *
 * Object iimplementation file
**/

#include "dice.h"
#include "object.h"
#include "ncurses.h"
#include "stdlib.h"
#include <cstring>
#include <string>
#include "stdio.h"

using namespace std;

object_t* object_f::make_widget()
{
    object_t* object = new object_t();

    object->name = this->name;
    object->description = this->description;
    object->type = this->type;

    if (this->type == "WEAPON") {
        object->representation = '|';
    } else if (this->type == "OFFHAND") {
        object->representation = ')';
    } else if (this->type == "RANGED") {
        object->representation = '}';
    } else if (this->type == "ARMOR") {
        object->representation = '[';
    } else if (this->type == "HELMET") {
        object->representation = ']';
    } else if (this->type == "CLOAK") {
        object->representation = '(';
    } else if (this->type == "GLOVES") {
        object->representation = '{';
    } else if (this->type == "BOOTS") {
        object->representation = '\\';
    } else if (this->type == "RING") {
        object->representation = '=';
    } else if (this->type == "AMULET") {
        object->representation = '"';
    } else if (this->type == "LIGHT") {
        object->representation = '_';
    } else if (this->type == "SCROLL") {
        object->representation = '~';
    } else if (this->type == "BOOK") {
        object->representation = '?';
    } else if (this->type == "FLASK") {
        object->representation = '!';
    } else if (this->type == "GOLD") {
        object->representation = '$';
    } else if (this->type == "AMMUNITION") {
        object->representation = '/';
    } else if (this->type == "FOOD") {
        object->representation = ',';
    } else if (this->type == "WAND") {
        object->representation = '-';
    } else if (this->type == "CONTAINER") {
        object->representation = '%';
    } else if (this->type == "STACK") {
        object->representation = '&';
    } else {
        object->representation = '*';
    }

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

    object->stack_size = 0;
    object->display = true;

    return object;
}

void object_t::convert_to_stack()
{
    this->stack_size = 1;
    this->stack = (object_t**) malloc(sizeof(object_t *) * this->stack_size);

    object_t* object = new object_t();

    object->pos_x = this->pos_x;
    object->pos_y = this->pos_y;

    object->name = this->name;
    object->description = this->description;
    object->type = this->type;
    object->color = this->color;
    object->hit = this->hit;
    object->damage = this->damage;
    object->dodge = this->dodge;
    object->defense = this->defense;
    object->weight = this->weight;
    object->speed = this->speed;
    object->attr = this->attr;
    object->value = this->value;
    object->representation = this->representation;

    object->stack_size = 0;
    object->display = false;

    this->name = "Stack";
    this->description = "Just an ordinary stack of magical objects.";
    this->representation = '&';
    this->type = "stack";

    this->stack[0] = object;
}

bool object_t::convert_from_stack()
{
    if (this->stack_size == 1) {
        object_t* object = this->stack[0];

        this->stack_size = 0;

        this->pos_x = object->pos_x;
        this->pos_y = object->pos_y;

        this->name = object->name;
        this->description = object->description;
        this->type = object->type;
        this->color = object->color;
        this->hit = object->hit;
        this->damage = object->damage;
        this->dodge = object->dodge;
        this->defense = object->defense;
        this->weight = object->weight;
        this->speed = object->speed;
        this->attr = object->attr;
        this->value = object->value;
        this->representation = object->representation;

        delete object;
        free(this->stack);

        this->stack_size = 0;

        return true;
    }

    return false;
}

bool object_t::add_to_stack(object_t* obj)
{
    if (this->stack_size == 0 && this->type != "stack") {
        return false;
    }

    obj->display = false;

    this->stack_size += 1;
    this->stack = (object_t**) realloc(this->stack, sizeof(object_t *) * this->stack_size);

    this->stack[stack_size - 1] = obj;
    this->color = obj->color;

    return true;
}

object_t* object_t::pick_from_top_of_stack()
{
    if (this->stack_size == 0 || this->type != "stack") {
        return NULL;
    }

    this->stack_size -= 1;

    return this->stack[stack_size];
}

object_t::~object_t()
{
    for (int j = 0; j < this->stack_size; j++) {
        printf("Deleting: %i\n", j);
        delete this->stack[j];
    }

    if (this->type == "stack") {
        free(this->stack);
    }
}
