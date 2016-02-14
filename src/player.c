#include <stdlib.h>
#include "player.h"
#include "map.h"
#include "utils.h"
#include "point.h"
#include "binheap.h"

void map_player_init(map* current)
{
    int room = rand() % current->room_count;

    int pos_x = current->rooms[room].pos_x + 1 + rand() % (current->rooms[room].width-2);
    int pos_y = current->rooms[room].pos_y + 1 + rand() % (current->rooms[room].height-2);

    current->main_character.pos_x = pos_x;
    current->main_character.pos_y = pos_y;

    current->characters_layer[pos_y][pos_x] = '@';
}

void map_player_distances(map* current)
{
    map_player_distances_blank(current);
    map_player_distances_bounded(current);
    map_player_distances_unbounded(current);
}

void map_player_distances_blank(map* current)
{
    current->main_character.player_distances = malloc(sizeof(char) * current->rows * current->cols);
    current->main_character.all_distances = malloc(sizeof(char) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        current->main_character.player_distances[y] = malloc(sizeof(char) * current->cols);
        current->main_character.all_distances[y] = malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            current->main_character.player_distances[y][x] = 255;
            current->main_character.all_distances[y][x] = 255;
        }
    }
}

void map_player_distances_bounded(map* current)
{
    point start = {current->main_character.pos_x, current->main_character.pos_y};
    binheap_t queue;
    binheap_init(&queue, point_distance, NULL);

    current->main_character.player_distances[start.y][start.x] = 0;
}

void map_player_distances_unbounded(map* current)
{
}

void map_player_deinit(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        free(current->main_character.player_distances[y]);
        free(current->main_character.all_distances[y]);
    }
    free(current->main_character.player_distances);
    free(current->main_character.all_distances);
}
