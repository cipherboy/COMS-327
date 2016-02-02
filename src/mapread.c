#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

int main(int argc, char* argv[])
{
    map r;
    /*map_init(&r);
    map_print(&r);
    map_write(&r, strcat(getenv("HOME"), "/.rlg327/dungeon"));
    */
    map_read(&r, strcat(getenv("HOME"), "/.rlg327/dungeon"));
    map_print(&r);
}
