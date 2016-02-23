
/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy implementation file
**/

#include <stdlib.h>

#include "map.h"
#include "map_enemy.h"

void map_enemy_init(map* current) {
    current->enemies = malloc(sizeof(enemy) * current->enemy_count);
}

void map_enemy_deinit(map* current) {
    free(current->enemies);
}
