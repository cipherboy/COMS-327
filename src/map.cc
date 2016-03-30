/**
 * Copyright 2016 Alexander Scheel
 *
 * Main map implementation file
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

void map_init(map_t* current)
{
    map_blank(current);
    map_fill(current);
    map_layers(current);
}

bool map_read(map_t* current, char* basepath)
{
    char magic[7];
    magic[6] = '\0';
    FILE* f_ptr;
    int fread_size = 0;

    char* path = (char *) malloc(sizeof(char) * 1100);
    strncat(path, basepath, 1024);
    strcat(path, "/dungeon");

    f_ptr = fopen(path, "r");
    if (f_ptr == NULL) {
        printf("Opening file for writing failed with code %d, %s\n", errno, strerror(errno));
        free(path);
        return false;
    }

    fread_size = fread(&magic, sizeof(char), 6, f_ptr);
    if (fread_size != 6 || strcmp(magic, "RLG327") != 0) {
        printf("Invalid magic bytes: %s\n", magic);
        free(path);
        return false;
    }

    uint32_t version;
    fread_size = fread(&version, sizeof(version), 1, f_ptr);
    version = be32toh(version);
    if (fread_size != 1 || version != 0) {
        printf("Invalid version number: %u\n", version);
        free(path);
        return false;
    }

    uint32_t file_size;
    fread_size = fread(&file_size, sizeof(file_size), 1, f_ptr);
    file_size = be32toh(file_size);
    if (fread_size != 1) {
        printf("Unable to read file size: %u\n", file_size);
        free(path);
        return false;
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
    current->rooms = (room *) malloc(sizeof(room*) * current->room_count);

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
    free(path);
    return true;
}

void map_blank(map_t* current)
{
    current->cols = 80;
    current->rows = 21;

    srand(utils_genseed());

    current->enemy_count = rand() % 15;

    current->rock_hardness = (uint8_t**) malloc(sizeof(uint8_t*) * current->rows * current->cols);
    current->characters_layer = (char**) malloc(sizeof(char*) * current->rows * current->cols);
    current->characters_location = (character_t***) malloc(sizeof(character_t**) * current->rows * current->cols);
    current->rooms_layer = (char**) malloc(sizeof(char*) * current->rows * current->cols);
    current->hallways_layer = (char**) malloc(sizeof(char*) * current->rows * current->cols);
    current->char_buffer = (char**) malloc(sizeof(char*) * current->rows * current->cols);
    for (int y = 0; y < current->rows; y++) {
        current->rock_hardness[y] = (uint8_t*) malloc(sizeof(uint8_t) * current->cols);
        current->characters_layer[y] = (char*) malloc(sizeof(char) * current->cols);
        current->characters_location[y] = (character_t**) malloc(sizeof(character_t*) * current->cols);
        current->rooms_layer[y] = (char*) malloc(sizeof(char) * current->cols);
        current->hallways_layer[y] = (char*) malloc(sizeof(char) * current->cols);
        current->char_buffer[y] = (char*) malloc(sizeof(char) * current->cols);
        for (int x = 0; x < current->cols; x++) {
            current->rock_hardness[y][x] = 255;
            current->characters_layer[y][x] = ' ';
            current->char_buffer[y][x] = ' ';
            current->characters_location[y][x] = NULL;
            current->rooms_layer[y][x] = ' ';
            current->hallways_layer[y][x] = ' ';
        }
    }
}

void map_fill(map_t* current)
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

        r_pos_x = 3 + (rand() % 65);
        r_pos_y = 1 + (rand() % 13);
        r_width = 5 + (rand() % 12);
        r_height = 4 + (rand() % 3);

        while (r_width + r_pos_x >= (current->cols-1) || r_height + r_pos_y >= (current->rows-1)) {
            r_pos_x = 3 + (rand() % 65);
            r_pos_y = 1 + (rand() % 13);
            r_width = 5 + (rand() % 12);
            r_height = 4 + (rand() % 3);
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
    current->rooms = (room*) malloc(sizeof(room) * r_num_rooms);

    for (int i = 0; i < r_num_rooms; i++) {
        current->rooms[i] = candidates[choices[i]];
        for (int j = 0; j < current->rooms[i].height; j++) {
            for (int k = 0; k < current->rooms[i].width; k++) {
                current->hallways_layer[current->rooms[i].pos_y + j][current->rooms[i].pos_x + k] = '#';
            }
        }
    }
}

void map_stairs(map_t* current)
{
    int uproom = map_rooms_find_contains_point(current, current->main_character->pos_x, current->main_character->pos_y);
    if (uproom == -1) {
        uproom = rand() % current->room_count;
    }

    int downroom = uproom;
    while (downroom == uproom) {
        downroom = rand() % current->room_count;
    }

    int dx = rand() % current->rooms[uproom].width;
    int dy = rand() % current->rooms[uproom].height;
    current->rooms_layer[current->rooms[uproom].pos_y + dy][current->rooms[uproom].pos_x + dx] = '<';

    dx = rand() % current->rooms[downroom].width;
    dy = rand() % current->rooms[downroom].height;
    current->rooms_layer[current->rooms[downroom].pos_y + dy][current->rooms[downroom].pos_x + dx] = '>';
}

void map_print(map_t* current)
{
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            int dx = current->main_character->pos_y - y;
            int dy = current->main_character->pos_x - x;

            if (current->characters_layer[y][x] != ' ' && dx >= -3 && dx <= 3 && dy >= -3 && dy <= 3) {
                current->char_buffer[y][x] = current->characters_layer[y][x];
                FILE* fp = fopen("/tmp/test.txt", "a");
                fprintf(fp, "dx: %i dy: %i :: %c\n", dx, dy, current->characters_layer[y][x]);
                fclose(fp);
            } else if ((current->main_character->seen_map[y][x]) != ' ') {
                current->char_buffer[y][x] = (current->main_character->seen_map[y][x]);
            } else {
                current->char_buffer[y][x] = ' ';
            }
        }
    }
}

void map_write(map_t* current, char* basepath)
{
    char magic[7] = "RLG327";
    uint32_t version = htobe32(0);
    uint32_t file_size = htobe32(6+4+4+1482+(4*current->room_count));

    char* path = (char *) malloc(sizeof(char) * 1100);
    strncat(path, basepath, 1024);
    strcat(path, "/dungeon");

    FILE* f_ptr;
    int fwrite_size = 0;

    f_ptr = fopen(path, "w");
    if (f_ptr == NULL) {
        printf("Opening file for writing failed with code %d, %s\n", errno, strerror(errno));
        free(path);
        return;
    }

    fwrite_size = fwrite(&magic, sizeof(char), 6, f_ptr);
    if (fwrite_size != 6) {
        printf("Unable to write magic bytes: %i != 6\n", fwrite_size);
        free(path);
        return;
    }

    fwrite_size = fwrite(&version, sizeof(version), 1, f_ptr);
    if (fwrite_size != 1) {
        printf("Unable to write version: %i != 1\n", fwrite_size);
        free(path);
        return;
    }

    fwrite_size = fwrite(&file_size, sizeof(file_size), 1, f_ptr);
    if (fwrite_size != 1) {
        printf("Unable to write file size: %i != 1\n", fwrite_size);
        free(path);
        return;
    }

    for (int y = 1; y < current->rows-1; y++) {
        for (int x = 1; x < current->cols-1; x++) {
            fwrite_size = fwrite(&current->rock_hardness[y][x], sizeof(current->rock_hardness[y][x]), 1, f_ptr);
            if (fwrite_size != 1) {
                printf("Unable to write hardness[%i][%i]: %i != 1\n", y, x, fwrite_size);
                free(path);
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
            free(path);
            return;
        }

        fwrite_size = fwrite(&r_pos_y, sizeof(r_pos_y), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_pos_y[%i]: %i != 1\n", room_counter, fwrite_size);
            free(path);
            return;
        }

        fwrite_size = fwrite(&r_width, sizeof(r_width), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_width[%i]: %i != 1\n", room_counter, fwrite_size);
            free(path);
            return;
        }

        fwrite_size = fwrite(&r_height, sizeof(r_height), 1, f_ptr);
        if (fwrite_size != 1) {
            printf("Unable to write r_height[%i]: %i != 1\n", room_counter, fwrite_size);
            free(path);
            return;
        }
    }

    fclose(f_ptr);
    free(path);
}

void map_deinit(map_t* current)
{
    for (int y = 0; y < current->rows; y++) {
        free(current->rock_hardness[y]);
        free(current->rooms_layer[y]);
        free(current->characters_layer[y]);
        free(current->characters_location[y]);
        free(current->hallways_layer[y]);
        free(current->char_buffer[y]);
    }

    free(current->rock_hardness);
    free(current->rooms_layer);
    free(current->hallways_layer);
    free(current->characters_layer);
    free(current->characters_location);
    free(current->char_buffer);
    free(current->rooms);
}
