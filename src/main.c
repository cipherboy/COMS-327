#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s [--save] [--load]\n\t--save\t- create a new world and save state\n\t--load\t-load an existing world\n", argv[0]);
        return 1;
    }

    map r;
    if (strcmp(argv[1], "--save") == 0) {
        map_init(&r);
        map_print(&r);
        map_write(&r, strcat(getenv("HOME"), "/.rlg327/dungeon"));
        return 0;
    } else if (strcmp(argv[1], "--load") == 0) {
        map_read(&r, strcat(getenv("HOME"), "/.rlg327/dungeon"));
        map_print(&r);
        return 0;
    } else {
        printf("Usage: %s [--save] [--load]\n\t--save\t- create a new world and save state\n\t--load\t-load an existing world\n", argv[0]);
        return 1;
    }
}
