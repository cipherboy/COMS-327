/**
 * Copyright 2016 Alexander Scheel
 *
 * Utils header file
**/

#pragma once
#ifndef UTILS_H
#define UTILS_H

# ifdef __cplusplus
extern "C" {
# endif

unsigned int utils_genseed();
int ipow(int base, int exp);
int raw_distances(int x1, int y1, int x2, int y2);

typedef void (*func_table_element)(map *, int);

# ifdef __cplusplus
}
# endif

#endif
