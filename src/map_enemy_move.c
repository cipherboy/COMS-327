/**
 * Copyright 2016 Alexander Scheel
 *
 * Map enemy move header file
**/

#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "map_enemy.h"
#include "map.h"
#include "utils.h"
#include "player.h"

void map_enemy_move_random(map* current, int enemy_loc)
{

    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;

    while (dy == 0 && dx == 0) {
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    }

    int pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
    int pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

    while (pos_x < 0 || pos_x >= current->cols || pos_y < 0 || pos_y >= current->rows) {
        while (dy == 0 && dx == 0) {
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
        }

        pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
        pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;
    }

    int tunneling = (*enemy_attributes(current->enemies[enemy_loc]) )& ENEMY_ATTRIBUTE_TUNNELING;
    if (tunneling) {
        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            if (current->rock_hardness[pos_y][pos_x] > 85) {
                current->rock_hardness[pos_y][pos_x] -= 85;
            } else {
                current->rock_hardness[pos_y][pos_x] = 0;
                current->hallways_layer[pos_y][pos_x] = '#';

                if (((*character_pos_x(current->enemies[enemy_loc]))!= pos_x || (*character_pos_y(current->enemies[enemy_loc])) != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                        (*character_is_alive(current->main_character)) = false;
                    }

                    (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
                }
                (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
                (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
            }

            map_player_distances(current);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                    (*character_is_alive(current->main_character)) = false;
                }

                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
            (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
        }
    } else {
        int tries = 0;
        while (current->rock_hardness[pos_y][pos_x] != 0 && tries <= 100) {
            while (dy == 0 && dx == 0) {
                dx = rand() % 3 - 1;
                dy = rand() % 3 - 1;
                tries += 1;
            }

            pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
            pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

            tries += 1;
        }

        if (tries >= 100) {
            return;
        }

        if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
            if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                (*character_is_alive(current->main_character)) = false;
            }

            (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
        }

        (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
        (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
    }
}

void map_enemy_move_erratic(map* current, int enemy_loc)
{
    if (rand() % 2 == 0) {
        map_enemy_move_random(current, enemy_loc);
    } else {
        (*enemy_attributes(current->enemies[enemy_loc])) -= ENEMY_ATTRIBUTE_ERRATIC;
        map_enemy_move(current, enemy_loc);
        (*enemy_attributes(current->enemies[enemy_loc])) += ENEMY_ATTRIBUTE_ERRATIC;
    }
}

void map_enemy_move_intelligent_telepathic(map* current, int enemy_loc)
{
    int tunneling = (*enemy_attributes(current->enemies[enemy_loc]) )& ENEMY_ATTRIBUTE_TUNNELING;

    if (tunneling) {
        int pos_x = (*character_pos_x(current->enemies[enemy_loc]));
        int pos_y = (*character_pos_y(current->enemies[enemy_loc]));
        int min_distance = current->cols * current->rows;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == dy && dx == 0) {
                    continue;
                }

                if (((*player_all_distances(current->main_character)))[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] < min_distance && current->rock_hardness[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] != 255) {
                    pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
                    pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

                    min_distance = ((*player_all_distances(current->main_character)))[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx];
                }
            }
        }

        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            if (current->rock_hardness[pos_y][pos_x] > 85) {
                current->rock_hardness[pos_y][pos_x] -= 85;
            } else {
                current->rock_hardness[pos_y][pos_x] = 0;
                current->hallways_layer[pos_y][pos_x] = '#';

                if (((*character_pos_x(current->enemies[enemy_loc])) != pos_x || (*character_pos_y(current->enemies[enemy_loc])) != pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                        (*character_is_alive(current->main_character)) = false;
                    }

                    (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
                }
                (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
                (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
            }

            map_player_distances(current);
        } else if (current->rock_hardness[pos_y][pos_x] == 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                    (*character_is_alive(current->main_character)) = false;
                }

                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
            (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
        }
    } else {
        int pos_x = (*character_pos_x(current->enemies[enemy_loc]));
        int pos_y = (*character_pos_y(current->enemies[enemy_loc]));
        int min_distance = current->cols * current->rows;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == dy && dx == 0) {
                    continue;
                }

                if (((*player_player_distances(current->main_character)))[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] < min_distance && current->rock_hardness[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] == 0) {
                    pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
                    pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

                    min_distance = ((*player_player_distances(current->main_character)))[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx];
                }
            }
        }

        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                    (*character_is_alive(current->main_character)) = false;
                }

                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
            (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
        }
    }
}

void map_enemy_move_intelligent_not_telepathic(map* current, int enemy_loc)
{
    map_enemy_update_last_seen(current, enemy_loc);

    if ((*enemy_has_seen_main_character(current->enemies[enemy_loc]) )&& (*enemy_main_character_last_x(current->enemies[enemy_loc]) )!= (*character_pos_x(current->enemies[enemy_loc]) )&& (*enemy_main_character_last_y(current->enemies[enemy_loc]) )!= (*character_pos_y(current->enemies[enemy_loc])) ) {
        map_enemy_move_intelligent_telepathic(current, enemy_loc);
    } else {
        // Make no move; the praying mantis strikes again.
    }
}

void map_enemy_move_not_intelligent_telepathic(map* current, int enemy_loc)
{
    map_enemy_update_last_seen(current, enemy_loc);

    int tunneling = (*enemy_attributes(current->enemies[enemy_loc]) )& ENEMY_ATTRIBUTE_TUNNELING;

    if (tunneling) {
        int pos_x = (*character_pos_x(current->enemies[enemy_loc]));
        int pos_y = (*character_pos_y(current->enemies[enemy_loc]));
        int min_distance = current->cols * current->rows;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == dy && dx == 0) {
                    continue;
                }

                int new_distance = raw_distances((*character_pos_x(current->enemies[enemy_loc])) + dx, (*character_pos_y(current->enemies[enemy_loc])) + dy, (*enemy_main_character_last_x(current->enemies[enemy_loc])), (*enemy_main_character_last_y(current->enemies[enemy_loc])));

                if (new_distance < min_distance && current->rock_hardness[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] != 255) {
                    pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
                    pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

                    min_distance = new_distance;
                }
            }
        }

        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            if (current->rock_hardness[pos_y][pos_x] > 85) {
                current->rock_hardness[pos_y][pos_x] -= 85;
            } else {
                current->rock_hardness[pos_y][pos_x] = 0;
                current->hallways_layer[pos_y][pos_x] = '#';

                if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                    if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                        (*character_is_alive(current->main_character)) = false;
                    }

                    (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
                }
                (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
                (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
            }

            map_player_distances(current);
        } else if (current->rock_hardness[pos_y][pos_x] == 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                    (*character_is_alive(current->main_character)) = false;
                }

                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
            (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
        }
    } else {
        int pos_x = (*character_pos_x(current->enemies[enemy_loc]));
        int pos_y = (*character_pos_y(current->enemies[enemy_loc]));
        int min_distance = current->cols * current->rows;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == dy && dx == 0) {
                    continue;
                }

                int new_distance = raw_distances((*character_pos_x(current->enemies[enemy_loc])) + dx, (*character_pos_y(current->enemies[enemy_loc])) + dy, (*enemy_main_character_last_x(current->enemies[enemy_loc])), (*enemy_main_character_last_y(current->enemies[enemy_loc])));


                if (new_distance < min_distance && current->rock_hardness[(*character_pos_y(current->enemies[enemy_loc])) + dy][(*character_pos_x(current->enemies[enemy_loc])) + dx] == 0) {
                    pos_x = (*character_pos_x(current->enemies[enemy_loc])) + dx;
                    pos_y = (*character_pos_y(current->enemies[enemy_loc])) + dy;

                    min_distance = new_distance;
                }
            }
        }

        if (current->rock_hardness[pos_y][pos_x] > 0 && current->rock_hardness[pos_y][pos_x] != 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 255) {
            map_enemy_move_random(current, enemy_loc);
        } else if (current->rock_hardness[pos_y][pos_x] == 0) {
            current->hallways_layer[pos_y][pos_x] = '#';

            if (((*character_pos_x(current->enemies[enemy_loc]) )!= pos_x || (*character_pos_y(current->enemies[enemy_loc]) )!= pos_y) && current->characters_location[pos_y][pos_x] != NULL) {
                if ((*character_pos_x(current->main_character)) == pos_x && (*character_pos_y(current->main_character)) == pos_y) {
                    (*character_is_alive(current->main_character)) = false;
                }

                (*character_is_alive(*((enemy **) current->characters_location[pos_y][pos_x]))) = false;
            }

            (*character_pos_x(current->enemies[enemy_loc])) = pos_x;
            (*character_pos_y(current->enemies[enemy_loc])) = pos_y;
        }
    }
}


void map_enemy_move_not_intelligent_not_telepathic(map* current, int enemy_loc)
{
    (*enemy_has_seen_main_character(current->enemies[enemy_loc])) = false;
    map_enemy_update_last_seen(current, enemy_loc);

    if ((*enemy_has_seen_main_character(current->enemies[enemy_loc])) ) {
        map_enemy_move_not_intelligent_telepathic(current, enemy_loc);
    } else {
        map_enemy_move_random(current, enemy_loc);
    }
}
