#include<stdio.h>
#include "map.h"

int main(int argc, char* argv[])
{
    map r;
    map_init(&r);
    printf("Seed: %u\n", r.seed);
}
