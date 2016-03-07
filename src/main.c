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
    map r;

    bool load = false;
    bool save = false;
    bool other = false;
    int nummon = -1;

    char* path;
    path = malloc(sizeof(char) * 1024);
    snprintf(path, 1024, "%s/.rlg327/dungeon", getenv("HOME"));

    // Argument parsing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--load") == 0) {
            load = true;
        } else if (strcmp(argv[i], "--save") == 0) {
            save = true;
        } else if (strcmp(argv[i], "--nummon") == 0) {
            nummon = atoi(argv[++i]);
            if (nummon < 0 || nummon > 100) {
                printf("Invalid number of monsters: %i ; 0 <= nummon <= 100\n", nummon);
                free(path);
                return 1;
            }
        } else {
            other = true;
        }
    }

    if (other) {
        printf("Usage: %s [--save] [--load] [--nummon int]\n\t--save\t\tcreate a new world and save state\n\t--load\t\tload an existing world\n\t--nummon\tspecify number of monsters\n", argv[0]);
        free(path);
        return 1;
    }

    if (load) {
        if (!map_read(&r, path)) {
            printf("Error reading path: %s. Are you sure it exists and is readable?\n", path);
            return 0;
        }
    } else {
        map_init(&r);
    }

    if (nummon != -1) {
        //printf("Loading %i monsters...\n", nummon);
        r.enemy_count = nummon;
    }

    map_player_init(&r);
    map_player_distances(&r);
    map_stairs(&r);

    map_enemy_init(&r);

    map_main(&r);

    if (save) {
        map_write(&r, path);
    }

    map_enemy_deinit(&r);
    map_deinit(&r);
    free(path);

    return 0;
}
