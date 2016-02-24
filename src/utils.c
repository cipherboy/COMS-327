/**
 * Copyright 2016 Alexander Scheel
 *
 * Utils implementation file
**/

#include <time.h>
#include <stdio.h>

unsigned int utils_genseed()
{
    unsigned int data;

    FILE *urandom;
    urandom = fopen("/dev/urandom", "r");

    int fread_size = fread(&data, sizeof(data), 1, urandom);

    fclose(urandom);

    if (fread_size < 1) {
        time_t current;
        time(&current);

        data += current;
        data *= current;
        data += current;
        data *= current;
        data += current;
    }

    return data;
}

int ipow(int base, int exp)
{
    int result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int raw_distances(int x1, int y1, int x2, int y2)
{
    return ipow(x1 - x2, 2) + ipow(y1 - y2, 2);
}
