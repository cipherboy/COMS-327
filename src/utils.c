/**
 * Copyright 2016 Alexander Scheel
**/

#include <time.h>
#include <stdio.h>

unsigned int utils_genseed()
{
    unsigned int data;
    FILE *urandom;
    urandom = fopen("/dev/urandom", "r");
    int fread_size = fread(&data, sizeof(data), 1, urandom);

    printf("fread_size: %i\n", fread_size);
    printf("fread_size: %u\n", data);

    fclose(urandom);

    if (fread_size < 1) {
        time_t current;
        time(&current);

        data += current;
        data *= current;
        data += current;
    }

    return data;
}
