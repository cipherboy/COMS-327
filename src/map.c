/**
 * Copyright 2016 Alexander Scheel
**/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef __APPLE__
#include "endian.h"
#else
#include <endian.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "room.h"
#include "map.h"
#include "utils.h"
#include "player.h"

void map_init(map* current)
{
    map_blank(current);
    map_fill(current);
    map_layers(current);
}

void map_read(map* current, char* path)
{
    char magic[7];
    magic[6] = '\0';
    FILE* f_ptr;
    int fread_size = 0;

    f_ptr = fopen(path, "r");
    if (f_ptr == NULL) {
        printf("Opening file for writing failed with code %d, %s\n", errno, strerror(errno));
        return;
    }

    fread_size = fread(&magic, sizeof(char), 6, f_ptr);
    if (fread_size != 6 || strcmp(magic, "RLG327") != 0) {
        printf("Invalid magic bytes: %s\n", magic);
        return;
    }

    uint32_t version;
    fread_size = fread(&version, sizeof(version), 1, f_ptr);
    version = be32toh(version);
    if (fread_size != 1 || version != 0) {
        printf("Invalid version number: %u\n", version);
        return;
    }

    uint32_t file_size;
    fread_size = fread(&file_size, sizeof(file_size), 1, f_ptr);
    file_size = be32toh(file_size);
    if (fread_size != 1) {
        printf("Unable to read file size: %u\n", file_size);
        return;
    }

    map_blank(current);

    for (int y = 1; y < current->rows-1; y++) {
        for (int x = 1; x < current->cols-1; x++) {
            current->rooms_layer[y][x] = ' ';
            current->hallways_layer[y][x] = ' ';

            uint8_t hardness;
            fread_size = fread(&hardness, sizeof(hardness), 1, f_ptr);

            if (fread_size != 1) {
                printf("Unable to read hardness %i\n", hardness);
            }

            current->rock_hardness[y][x] = hardness;

            if (hardness == 0) {
                current->hallways_layer[y][x] = '#';
            }
        }
    }

    current->room_count = (file_size-1495)/4;
    current->rooms = malloc(sizeof(room) * current->room_count);

    for (int i = 0; i < current->room_count; i++) {
        room current_room;

        uint8_t r_pos_x = 0;
        uint8_t r_pos_y = 0;
        uint8_t r_width = 0;
        uint8_t r_height = 0;
        fread_size = fread(&r_pos_x, sizeof(r_pos_x), 1, f_ptr);
        if (fread_size != 1) {
            printf("Unable to read r_pos_x: %i\n", r_pos_x);
        }
        fread_size = fread(&r_pos_y, sizeof(r_pos_y), 1, f_ptr);
        if (fread_size != 1) {
            printf("Unable to read r_pos_y: %i\n", r_pos_y);
        }
        fread_size = fread(&r_width, sizeof(r_width), 1, f_ptr);
        if (fread_size != 1) {
            printf("Unable to read r_width: %i\n", r_width);
        }
        fread_size = fread(&r_height, sizeof(r_height), 1, f_ptr);
        if (fread_size != 1) {
            printf("Unable to read r_height: %i\n", r_height);
        }

        current_room.pos_x = r_pos_x;
        current_room.pos_y = r_pos_y;
        current_room.height = r_height;
        current_room.width = r_width;
        current_room.max_x = r_pos_x + r_width;
        current_room.max_y = r_pos_y + r_height;

        current->rooms[i] = current_room;
    }

    fclose(f_ptr);

    map_layers_rooms(current);
}

void map_blank(map* current)
{
    current->cols = 80;
    current->rows = 21;

    srand(utils_genseed());

    current->rock_hardness = malloc(sizeof(uint8_t) * current->rows * current->cols);
    current->characters_layer = malloc(sizeof(char) * current->rows * current->cols);
    current->rooms_layer = malloc(sizeof(char) * current->rows * current->cols);
    current->hallways_layer = malloc(sizeof(char) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        current->rock_hardness[y] = malloc(sizeof(uint8_t) * current->cols);
        current->characters_layer[y] = malloc(sizeof(char) * current->cols);
        current->rooms_layer[y] = malloc(sizeof(char) * current->cols);
        current->hallways_layer[y] = malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            current->rock_hardness[y][x] = 255;
            current->characters_layer[y][x] = ' ';
            current->rooms_layer[y][x] = ' ';
            current->hallways_layer[y][x] = ' ';
        }
    }
}

void map_fill(map* current)
{
    for (int y = 1; y < current->rows-1; y++) {
        for (int x = 1; x < current->cols-1; x++) {
            current->rock_hardness[y][x] = 1 + (rand() % 254);
        }
    }

    int r_rooms_generated = ((rand() % (5*current->rows*current->cols)) + (5*current->rows*current->cols)) % (5*current->rows*current->cols);
    r_rooms_generated += 3*current->rows*current->cols;

    room candidates[r_rooms_generated];

    // 3 <= pos_x <= 71
    // 2 <= pos_y <= 16
    // 3 <= width <= 15 ; pos_x + width < current->cols
    // 2 <= height <= 5 ; pos_y + height < current->rows

    for (int i = 0; i < r_rooms_generated; i++) {
        uint8_t r_pos_x = 3;
        uint8_t r_pos_y = 1;
        uint8_t r_width = 5;
        uint8_t r_height = 4;

        r_pos_x += rand() % 68;
        r_pos_y += rand() % 14;
        r_width += rand() % 12;
        r_height += rand() % 3;

        if (r_width + r_pos_x >= current->cols) {
            r_width = 3;
        }

        if (r_height + r_pos_y >= current->rows) {
            r_height = 2;
        }

        candidates[i].pos_x = r_pos_x;
        candidates[i].pos_y = r_pos_y;
        candidates[i].height = r_height;
        candidates[i].width = r_width;
        candidates[i].max_x = r_pos_x + r_width;
        candidates[i].max_y = r_pos_y + r_height;
    }

    // 5 <= r_num_rooms <= 8
    int r_num_rooms = 5;
    r_num_rooms += rand() % 3;

    int choices[r_num_rooms];

    for (int i = 0; i < r_num_rooms; i++) {
        bool has_intersection = false;
        int tries = 0;
        do {
            choices[i] = (((rand() + rand()) % r_rooms_generated) + r_rooms_generated) % r_rooms_generated;

            has_intersection = false;
            room current = candidates[choices[i]];
            for (int j = 0; j < i; j++) {
                room past = candidates[choices[j]];

                has_intersection = room_overlaps(&current, &past);
                if (has_intersection) {
                    choices[i] = (((rand() + rand()) % r_rooms_generated) + r_rooms_generated) % r_rooms_generated;
                    break;
                }
            }

            tries += 1;

            if (tries >= r_rooms_generated) {
                i = 0;
                tries = 0;
                break;
            }
        } while (has_intersection);
    }

    current->room_count = r_num_rooms;
    current->rooms = malloc(sizeof(room) * r_num_rooms);

    for (int i = 0; i < r_num_rooms; i++) {
        current->rooms[i] = candidates[choices[i]];
    }
}


void map_layers(map* current)
{
    map_layers_rooms(current);
    map_layers_hallways(current);
}

void map_layers_rooms(map* current)
{
    for (int i = 0; i < current->room_count; i++) {
        room current_room = current->rooms[i];

        for (int y = current_room.pos_y; y <= current_room.max_y; y++) {
            for (int x = current_room.pos_x; x < current_room.max_x; x++) {
                current->rooms_layer[y][x] = '.';
                current->rock_hardness[y][x] = 0;
            }
        }
    }
}

void map_layers_hallways(map* current)
{
    // Adjacency matrix for rooms...
    bool connected[current->room_count][current->room_count];
    for (int i = 0; i < current->room_count; i++) {
        for (int j = 0; j < current->room_count; j++) {
            connected[i][j] = false;
        }
    }

    for (int i = 0; i < current->room_count; i++) {
        room current_room = current->rooms[i];

        for (int j = 0; j < current->room_count; j++) {
            if (connected[i][j]) {
                break;
            }

            room referenced_room = current->rooms[j];
            int cy = current_room.pos_y + (current_room.height/2);
            int cx = current_room.pos_x + (current_room.width/2);
            int ry = referenced_room.pos_y + (referenced_room.height/2);
            int rx = referenced_room.pos_x + (referenced_room.width/2);

            int direction_x = (rx-cx);
            if (direction_x < 0) {
                direction_x = -1;
            } else if (direction_x > 0) {
                direction_x = 1;
            }

            int direction_y = (ry-cy);
            if (direction_y < 0) {
                direction_y = -1;
            } else if (direction_y > 0) {
                direction_y = 1;
            }

            int y = cy;
            int x = cx;

            if (rand() % (current->room_count-2)) {
                for (x = cx; x != rx; x += direction_x) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
                for (y = cy; y != ry; y += direction_y) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
            } else {
                for (y = cy; y != ry; y += direction_y) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
                for (x = cx; x != rx; x += direction_x) {
                    current->hallways_layer[y][x] = '#';
                    current->rock_hardness[y][x] = 0;
                }
            }

            connected[i][j] = true;
            connected[j][i] = true;
        }
    }
}

void map_print(map* current)
{
    printf("*--------------------------------------------------------------------------------*\n");
    for (int y = 0; y < current->rows; y++) {
        printf("|");
        for (int x = 0; x < current->cols; x++) {
            if (current->characters_layer[y][x] != ' ') {
                printf("%c", current->characters_layer[y][x]);
            } else if (current->rooms_layer[y][x] != ' ') {
                printf("%c", current->rooms_layer[y][x]);
            } else if (current->hallways_layer[y][x] != ' ') {
                printf("%c", current->hallways_layer[y][x]);
            } else {
                printf("%c", ' ');
            }
        }
        printf("|\n");
    }
    printf("*--------------------------------------------------------------------------------*\n");
    printf("\n\n\n");
    char distanceCharacter[62] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    printf("*--------------------------------------------------------------------------------*\n");
    for (int y = 0; y < current->rows; y++) {
        printf("|");
        for (int x = 0; x < current->cols; x++) {
            if (current->main_character.player_distances[y][x] < 62 && current->main_character.player_distances[y][x] >= 0) {
                printf("%c", distanceCharacter[(int) current->main_character.player_distances[y][x]]);
            } else if (current->rooms_layer[y][x] != ' ') {
                printf("%c", current->rooms_layer[y][x]);
            } else if (current->hallways_layer[y][x] != ' ') {
                printf("%c", current->hallways_layer[y][x]);
            } else {
                printf("%c", ' ');
            }
        }
        printf("|\n");
    }
    printf("*--------------------------------------------------------------------------------*\n");
    printf("\n\n\n");

    printf("*--------------------------------------------------------------------------------*\n");
    for (int y = 0; y < current->rows; y++) {
        printf("|");
        for (int x = 0; x < current->cols; x++) {
            if (current->main_character.all_distances[y][x] < 62 && current->main_character.all_distances[y][x] >= 0) {
                printf("%c", distanceCharacter[(int) current->main_character.all_distances[y][x]]);
            } else if (current->rooms_layer[y][x] != ' ') {
                printf("%c", current->rooms_layer[y][x]);
            } else if (current->hallways_layer[y][x] != ' ') {
                printf("%c", current->hallways_layer[y][x]);
            } else {
                printf("%c", ' ');
            }
        }
        printf("|\n");
    }
    printf("*--------------------------------------------------------------------------------*\n");
    printf("\n\n\n");
}

void map_write(map* current, char* path)
{
    char magic[7] = "RLG327";
    uint32_t version = htobe32(0);
    uint32_t file_size = htobe32(6+4+4+1482+(4*current->room_count));

    FILE* f_ptr;
    int fwrite_size = 0;

    f_ptr = fopen(path, "w");
    if (f_ptr == NULL) {
        printf("Opening file for writing failed with code %d, %s\n", errno, strerror(errno));
        return;
    }

    fwrite_size = fwrite(&magic, sizeof(char), 6, f_ptr);
    if (fwrite_size != 6) {
        printf("Unable to write magic bytes: %i != 6\n", fwrite_size);
        return;
    }

    fwrite_size = fwrite(&version, sizeof(version), 1, f_ptr);
    if (fwrite_size != 1) {
        printf("Unable to write version: %i != 1\n", fwrite_size);
        return;
    }

    fwrite_size = fwrite(&file_size, sizeof(file_size), 1, f_ptr);
    if (fwrite_size != 1) {
        printf("Unable to write file size: %i != 1\n", fwrite_size);
        return;
    }

    for (int y = 1; y < current->rows-1; y++) {
        for (int x = 1; x < current->cols-1; x++) {
            fwrite_size = fwrite(&current->rock_hardness[y][x], sizeof(current->rock_hardness[y][x]), 1, f_ptr);
            if (fwrite_size != 1) {
                printf("Unable to write hardness[%i][%i]: %i != 1\n", y, x, fwrite_size);
                return;
            }
        }
    }

    for (int room_counter = 0; room_counter < current->room_count; room_counter++) {
        uint8_t r_pos_x = current->rooms[room_counter].pos_x;
        uint8_t r_pos_y = current->rooms[room_counter].pos_y;
        uint8_t r_width = current->rooms[room_counter].width;
        uint8_t r_height = current->rooms[room_counter].height;

        fwrite_size = fwrite(&r_pos_x, sizeof(r_pos_x), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_pos_x[%i]: %i != 1\n", room_counter, fwrite_size);
            return;
        }

        fwrite_size = fwrite(&r_pos_y, sizeof(r_pos_y), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_pos_y[%i]: %i != 1\n", room_counter, fwrite_size);
            return;
        }

        fwrite_size = fwrite(&r_width, sizeof(r_width), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_width[%i]: %i != 1\n", room_counter, fwrite_size);
            return;
        }

        fwrite_size = fwrite(&r_height, sizeof(r_height), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_height[%i]: %i != 1\n", room_counter, fwrite_size);
            return;
        }
    }

    fclose(f_ptr);
}

void map_deinit(map* current)
{
    for (int y = 0; y < current->rows; y++) {
        free(current->rock_hardness[y]);
        free(current->rooms_layer[y]);
        free(current->characters_layer[y]);
        free(current->hallways_layer[y]);
    }

    free(current->rock_hardness);
    free(current->rooms_layer);
    free(current->hallways_layer);
    free(current->characters_layer);
    free(current->rooms);

    map_player_deinit(current);
}
