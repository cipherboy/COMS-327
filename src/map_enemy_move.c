/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy move header file
**/

#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "map.h"
#include "player.h"
#include "map_enemy.h"

void map_enemy_move_random(map* current, int enemy_loc)
{

    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;

    while (dy == 0 && dx == 0) {
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    }

    int pos_x = current->enemies[enemy_loc].pos_x + dx;
    int pos_y = current->enemies[enemy_loc].pos_y + dy;

    while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        pos_x = current->enemies[enemy_loc].pos_x + dx;
        pos_y = current->enemies[enemy_loc].pos_y + dy;
    }

    int tunneling = current->enemies[enemy_loc].attributes & ENEMY_ATTRIBUTE_TUNNELING;
    if (tunneling) {
        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            if (current->rock_hardness[pos_y][pos_x] > 85) {
                current->rock_hardness[pos_y][pos_x] -= 85;
            } else {
                current->rock_hardness[pos_y][pos_x] = 0;
                current->hallways_layer[pos_y][pos_x] = '#';

                if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                        current->main_character.is_alive = false;
                    }

                    ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
                }
                current->enemies[enemy_loc].pos_x = pos_x;
                current->enemies[enemy_loc].pos_y = pos_y;
            }

            map_player_distances(current);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                    current->main_character.is_alive = false;
                }

                ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
            }

            current->enemies[enemy_loc].pos_x = pos_x;
            current->enemies[enemy_loc].pos_y = pos_y;
        }
    } else {
        int tries = 0;
        while (current->rock_hardness[pos_y][pos_x] != 0 && tries <= 100) {
            while (dy == 0 && dx == 0) {
                dx = rand() % 3 - 1;
                dy = rand() % 3 - 1;
                tries += 1;
            }

            pos_x = current->enemies[enemy_loc].pos_x + dx;
            pos_y = current->enemies[enemy_loc].pos_y + dy;

            tries += 1;
        }

        if (tries >= 100) {
            printf("move_random: Unable to find suitable path!\n");
            return;
        }

        if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
            if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                current->main_character.is_alive = false;
            }

            ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
        }

        current->enemies[enemy_loc].pos_x = pos_x;
        current->enemies[enemy_loc].pos_y = pos_y;
    }
}

void map_enemy_move_erratic(map* current, int enemy_loc)
{
    if (rand() % 2 == 0) {
        map_enemy_move_random(current, enemy_loc);
    } else {
        current->enemies[enemy_loc].attributes -= ENEMY_ATTRIBUTE_ERRATIC;
        map_enemy_move(current, enemy_loc);
        current->enemies[enemy_loc].attributes += ENEMY_ATTRIBUTE_ERRATIC;
    }
}

void map_enemy_move_intelligent(map* current, int enemy_loc)
{
    int telepathic = current->enemies[enemy_loc].attributes & ENEMY_ATTRIBUTE_TELEPATHY;
    int tunneling = current->enemies[enemy_loc].attributes & ENEMY_ATTRIBUTE_TUNNELING;
    printf("move_intelligent: %i, %i, %i\n", telepathic, tunneling, current->enemies[enemy_loc].attributes);
    if (telepathic) {
        if (tunneling) {
            int pos_x = current->enemies[enemy_loc].pos_x;
            int pos_y = current->enemies[enemy_loc].pos_y;
            int min_distance = current->cols * current->rows;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == dy && dx == 0) {
                        continue;
                    }

                    if (current->main_character.all_distances[current->enemies[enemy_loc].pos_y + dy][current->enemies[enemy_loc].pos_x + dx] < min_distance && current->rock_hardness[current->enemies[enemy_loc].pos_y + dy][current->enemies[enemy_loc].pos_x + dx] != 255) {
                        pos_x = current->enemies[enemy_loc].pos_x + dx;
                        pos_y = current->enemies[enemy_loc].pos_y + dy;
                    }
                }
            }

            if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
                if (current->rock_hardness[pos_y][pos_x] > 85) {
                    current->rock_hardness[pos_y][pos_x] -= 85;
                } else {
                    current->rock_hardness[pos_y][pos_x] = 0;
                    current->hallways_layer[pos_y][pos_x] = '#';

                    if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                        if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                            current->main_character.is_alive = false;
                        }

                        ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
                    }
                    current->enemies[enemy_loc].pos_x = pos_x;
                    current->enemies[enemy_loc].pos_y = pos_y;
                }

                map_player_distances(current);
            } else if (current->rock_hardness[pos_y][pos_x] == 255) {
                printf("Random move!!!!\n");
                map_enemy_move_random(current, enemy_loc);
            } else if (current->rock_hardness[pos_y][pos_x] == 0) {
                current->hallways_layer[pos_y][pos_x] = '#';

                if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                        current->main_character.is_alive = false;
                    }

                    ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
                }

                current->enemies[enemy_loc].pos_x = pos_x;
                current->enemies[enemy_loc].pos_y = pos_y;
            }
        } else {
            int pos_x = current->enemies[enemy_loc].pos_x;
            int pos_y = current->enemies[enemy_loc].pos_y;
            int min_distance = current->cols * current->rows;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == dy && dx == 0) {
                        continue;
                    }

                    if (current->main_character.player_distances[current->enemies[enemy_loc].pos_y + dy][current->enemies[enemy_loc].pos_x + dx] < min_distance && current->rock_hardness[current->enemies[enemy_loc].pos_y + dy][current->enemies[enemy_loc].pos_x + dx] == 0) {
                        pos_x = current->enemies[enemy_loc].pos_x + dx;
                        pos_y = current->enemies[enemy_loc].pos_y + dy;
                    }
                }
            }

            if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
                printf("Cannot move...");
                map_enemy_move_random(current, enemy_loc);
            } else if (current->rock_hardness[pos_y][pos_x] == 255) {
                printf("Random move!!!!\n");
                map_enemy_move_random(current, enemy_loc);
            } else if (current->rock_hardness[pos_y][pos_x] == 0) {
                current->hallways_layer[pos_y][pos_x] = '#';

                if ((current->enemies[enemy_loc].pos_x != pos_x || current->enemies[enemy_loc].pos_y != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if (current->main_character.pos_x == pos_x && current->main_character.pos_y == pos_y) {
                        current->main_character.is_alive = false;
                    }

                    ((enemy *) current->characters_location[pos_y][pos_x])->is_alive = false;
                }

                current->enemies[enemy_loc].pos_x = pos_x;
                current->enemies[enemy_loc].pos_y = pos_y;
            }
        }
    }
}
