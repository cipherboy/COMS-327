/**
 * Copyright 2016 Alexander Scheel
 *
 * main method
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../src/map.h"
#include "../src/player.h"
#include "../src/character.h"

int main(int argc, char* argv[])
{
    map_c r;

    bool load = false;
    bool save = false;
    bool other = false;
    int nummon = -1;

    char* basepath;
    basepath = (char *) malloc(sizeof(char) * 1024);
    snprintf(basepath, 1024, "%s/.rlg327/", getenv("HOME"));

    // Argument parsing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--load") == 0) {
            load = true;
        } else if (strcmp(argv[i], "--save") == 0) {
            save = true;
        } else if (strcmp(argv[i], "--monsters") == 0) {
            printf("Depricated flag.\n");
            other = true;
        } else if (strcmp(argv[i], "--items") == 0) {
            printf("Depricated flag.\n");
            other = true;
        } else if (strcmp(argv[i], "--nummon") == 0) {
            nummon = atoi(argv[++i]);
            if (nummon < 0 || nummon > 100) {
                printf("Invalid number of monsters: %i ; 0 <= nummon <= 100\n", nummon);
                free(basepath);
                return 1;
            }
        } else {
            other = true;
        }
    }

    if (other) {
        printf("Usage: %s [--save] [--load] [--nummon int]\n\t--save\t\tcreate a new world and save state\n\t--load\t\tload an existing world\n\t--nummon\tspecify number of monsters\n\t--monsters\tparse the monsters definition file\n\t--items\t\tparse the items definition file\n\n", argv[0]);
        free(basepath);
        return 1;
    }

    if (load) {
        if (!map_read(&r, basepath)) {
            printf("Error reading basepath: %s. Are you sure it exists and is readable?\n", basepath);
            free(basepath);
            return 0;
        }
    } else {
        map_init(&r);
    }

    r.monster_factories = map_monster_parse_file(&r, basepath,  &(r.monster_factory_count));
    r.object_factories = map_item_parse_file(&r, basepath, &(r.object_factory_count));

    if (nummon != -1) {
        r.enemy_count = nummon;
    }

    player_t* player = new player_t(&r);

    map_player_init(&r, player);
    map_player_distances(&r);
    map_stairs(&r);

    map_enemy_init(&r);

    map_objects_init(&r);

    int rval = map_main(&r, true);

    while (rval != 0 && rval != -1) {
        map_objects_deinit(&r);
        map_enemy_deinit(&r);
        map_deinit(&r);

        map_init(&r);

        r.monster_factories = map_monster_parse_file(&r, basepath,  &(r.monster_factory_count));
        r.object_factories = map_item_parse_file(&r, basepath, &(r.object_factory_count));

        if (nummon != -1) {
            r.enemy_count = nummon;
        }

        r.main_character = player;
        map_player_distances(&r);
        map_stairs(&r);

        map_enemy_init(&r);
        map_objects_init(&r);

        rval = map_main(&r, false);
    }

    if (save) {
        map_write(&r, basepath);
    }

    map_objects_deinit(&r);
    map_enemy_deinit(&r);
    map_player_deinit(&r);
    map_deinit(&r);
    free(basepath);

    return 0;
}
