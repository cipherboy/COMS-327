#include<stdio.h>
#include "map.h"

int main(int argc, char* argv[])
{
    map r;
    map_read(getenv("HOME") + ".rlg327/dungeon");
    map_print(&r);
}
