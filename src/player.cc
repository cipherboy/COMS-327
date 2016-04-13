/**
 * Copyright 2016 Alexander Scheel
 *
 * Enemy implementation file
**/

#include "stdlib.h"
#include "player.h"
#include "character.h"
#include "map.h"
#include "object.h"

player_t::player_t(map_c* current) : character_t(current)
{
    this->player_distances = (char **) malloc(sizeof(char*) * current->rows * current->cols);
    this->all_distances = (char **) malloc(sizeof(char*) * current->rows * current->cols);
    this->seen_map = (char **) malloc(sizeof(char*) * current->rows * current->cols);
    for (int y = 0; y < this->current->rows; y++) {
        this->player_distances[y] = (char *) malloc(sizeof(char) * current->cols);
        this->all_distances[y] = (char *) malloc(sizeof(char) * current->cols);
        this->seen_map[y] = (char *) malloc(sizeof(char) * current->cols);
        for (int x = 0; x < this->current->cols; x++) {
            this->seen_map[y][x] = ' ';
        }
    }

    this->inventory = new object_t();
    this->inventory->display = false;
    this->inventory->name = "Player Inventory";
    this->inventory->description = "This is the stack containing the player's inventory.";
    this->inventory->type = "stack";
    this->inventory->representation = '&';
}

player_t::~player_t()
{
    for (int y = 0; y < this->current->rows; y++) {
        free(this->player_distances[y]);
        free(this->all_distances[y]);
        free(this->seen_map[y]);
    }
    free(this->player_distances);
    free(this->all_distances);
    free(this->seen_map);

    delete this->inventory;
}
