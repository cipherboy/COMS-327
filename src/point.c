#include <stdint.h>
#include "point.h"

int32_t point_distance(const void *key, const void *with)
{
    point* x = (point*) key;
    point* y = (point*) with;
    int32_t diff_x = (x->x - y->x);
    int32_t diff_y = (x->y - y->y);

    return (diff_x*diff_x) - (diff_y*diff_y);
}
