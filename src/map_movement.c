/**
 * Copyright 2016 Alexander Scheel
 *
 * Map layers header file
**/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

#include "map.h"
#include "binheap.h"
#include "node_moveable.h"

void map_main(map* current)
{

    map_render_splash();

    initscr();
    clear();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    int row, col;
    getmaxyx(stdscr,row,col);
    if (col < 79 || row < 23) {
        printw("\nPlease make your console at least 80x24: %i,%i.\n", row, col);
        getch();
        endwin();
        return;
    }

    binheap_t queue;
    binheap_init(&queue, moveables_moveable, NULL);

    moveable* objects = malloc(sizeof(moveable) * (current->enemy_count+1));
    binheap_node_t** bh_ptr = malloc(sizeof(binheap_node_t) * (current->enemy_count+1));

    objects[0].order = 0;
    objects[0].is_player = true;
    objects[0].speed = current->main_character.speed;
    objects[0].next_turn = 0;
    bh_ptr[0] = binheap_insert(&queue, &objects[0]);

    for (int i = 0; i < current->enemy_count; i++) {
        objects[i+1].order = i+1;
        objects[i+1].is_player = false;
        objects[i+1].speed = current->enemies[i].speed;
        objects[i+1].next_turn = 0;
        bh_ptr[i+1] = binheap_insert(&queue, &objects[i+1]);
    }

    map_enemy_render(current);

    map_print(current);
    for (int y = 0; y < current->rows; y++) {
        for (int x = 0; x < current->cols; x++) {
            mvaddch(y, x, current->char_buffer[y][x]);
        }
    }
    refresh();

    moveable* c;

    while (true) {
        c = (moveable*) binheap_remove_min(&queue);

        objects[c->order].next_turn += 100/c->speed;

        if (!c->is_player && current->enemies[c->order-1].is_alive) {
            map_enemy_move(current, c->order-1);
            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        } else if (c->is_player) {
            bool valid_key = false;
            int ch = 0;
            while (!valid_key) {
                ch = getch();
                valid_key = true;
                switch (ch) {
                case 109: // m -- monsters list
                    map_display_enemies(current);
                    break;
                case '7':
                case 'y':
                case KEY_HOME:
                    map_player_move(current, -1, -1);
                    break;
                case '8':
                case 'k':
                case KEY_UP:
                    map_player_move(current, 0, -1);
                    break;
                case '9':
                case 'u':
                case KEY_PPAGE:
                    map_player_move(current, 1, -1);
                    break;
                case '4':
                case 'h':
                case KEY_LEFT:
                    map_player_move(current, -1, 0);
                    break;
                case '5':
                case ' ':
                    // map_player_move(current, 0, 0);
                    break;
                case '6':
                case 'l':
                case KEY_RIGHT:
                    map_player_move(current, 1, 0);
                    break;
                case '1':
                case 'b':
                case KEY_END:
                    map_player_move(current, -1, 1);
                    break;
                case '2':
                case 'j':
                case KEY_DOWN:
                    map_player_move(current, 0, 1);
                    break;
                case '3':
                case 'n':
                case KEY_NPAGE:
                    map_player_move(current, 1, 1);
                    break;
                case 'S':
                case 's':
                    // Save to disk and exit game. Saving and restoring will be revisited later. For now, this will
                    // just quit the game.
                    // ~~~~~~~~~~~~~~~~~~
                    // "just quit the game"

                    printw("\nGame quit.\n");
                    getch();
                    endwin();

                    binheap_delete(&queue);

                    free(objects);
                    free(bh_ptr);

                    return;
                    break;
                default:
                    valid_key = false;
                }
            }

            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        }

        for (int y = 0; y < current->rows; y++) {
            for (int x = 0; x < current->cols; x++) {
                current->characters_layer[y][x] = ' ';
            }
        }

        if (!current->main_character.is_alive) {
            printw("\nMain character died! :(\n");
            break;
        } else {
            bool has_enemies = false;;
            for (int i = 0; i < current->enemy_count; i++) {
                if (current->enemies[i].is_alive) {
                    has_enemies = true;
                }
            }

            if (!has_enemies) {
                printw("\nMain character won!\n");
                break;
            }
        }

        map_enemy_render(current);

        map_print(current);
        for (int y = 0; y < current->rows; y++) {
            for (int x = 0; x < current->cols; x++) {
                mvaddch(y, x, current->char_buffer[y][x]);
            }
        }
        refresh();
    }

    getch();
    endwin();


    binheap_delete(&queue);

    free(objects);
    free(bh_ptr);
}

void map_display_enemies(map* current)
{
    // pass
}

void map_render_splash()
{
    initscr();
    int row, col;
    getmaxyx(stdscr,row,col);
    if (col < 79 || row < 23) {
        return;
    }
    printw("*-----------------------------------------------------------------------------*\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    printw("|                           |_|_|_  |_|_|_  |_                                |\n");
    printw("|                           |_      |_      |_                                |\n");
    printw("|                           |_      |_|_    |_                                |\n");
    printw("|                           |_      |_      |_                                |\n");
    printw("|                           |_|_|_  |_|_|_  |_|_|_                            |\n");
    printw("|                                                                             |\n");
    printw("|                           cipherboy~                                        |\n");
    printw("|                                entertainment                                |\n");
    printw("|                                          limited                            |\n");
    printw("|                                                                             |\n");
    printw("|                           v1.0%i ~ \"LoP\"                                     |\n", 5);
    printw("|                                                                             |\n");
    printw("|                         Optimized for POWER7+/POWER 8                       |\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    printw("|                                                     COPYRIGHT (C) 2016      |\n");
    printw("|                                                     ALL RIGHTS RESERVED     |\n");
    printw("|                                                     (see LICENSE for terms) |\n");
    printw("*-----------------------------------------------------------------------------*\n");
    refresh();
    sleep(3);
    endwin();
}
