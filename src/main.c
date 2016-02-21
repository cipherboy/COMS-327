/**
 * Copyright 2016 Alexander Scheel
 *
 * main method
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "map.h"

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3) {
        printf("Usage: %s [--save] [--load]\n\t--save\t- create a new world and save state\n\t--load\t-load an existing world\n", argv[0]);
        return 1;
    }

    map r;
    bool load = false;
    bool save = false;
    bool other = false;
    char* path;
    path = malloc(sizeof(char) * 1024);
    snprintf(path, 1024, "%s/.rlg327/dungeon", getenv("HOME"));

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--load") == 0) {
            load = true;
        } else if (strcmp(argv[i], "--save") == 0) {
            save = true;
        } else {
            other = true;
        }
    }

    if ((!load && !save) || other) {
        printf("Usage: %s [--save] [--load]\n\t--save\t- create a new world and save state\n\t--load\t-load an existing world\n", argv[0]);
        map_deinit(&r);
        free(path);
        return 1;
    }

    if (load) {
        if (!map_read(&r, path)) {
            printf("Error reading path: %s. Are you sure it exists and is readable?", path);
            return 0;
        }
    } else {
        map_init(&r);
    }
    map_player_init(&r);
    map_player_distances(&r);
    map_print(&r);

    if (save) {
        map_write(&r, path);
    }


    map_deinit(&r);
    free(path);
    return 0;
}
