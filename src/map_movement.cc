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
#include "character.h"
#include "player.h"
#include "enemy.h"
#include "binheap.h"
#include "node_moveable.h"

int map_main(map_c* current, bool splash)
{
    if (splash) {
        map_render_splash();
    }

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
        return -1;
    }

    if (has_colors() == FALSE) {
        printw("\nPlease make sure your console supports colors.\n");
        getch();
        endwin();
        return -1;
    }

    binheap_t queue;
    binheap_init(&queue, moveables_moveable, NULL);

    moveable* objects = (moveable*) malloc(sizeof(moveable) * (current->enemy_count+1));
    binheap_node_t** bh_ptr = (binheap_node_t**) malloc(sizeof(binheap_node_t*) * (current->enemy_count+1));

    objects[0].order = 0;
    objects[0].is_player = true;
    objects[0].speed = current->main_character->speed;
    objects[0].next_turn = 0;
    bh_ptr[0] = binheap_insert(&queue, &objects[0]);

    for (int i = 0; i < current->enemy_count; i++) {
        objects[i+1].order = i+1;
        objects[i+1].is_player = false;
        objects[i+1].speed = current->enemies[i]->speed;
        objects[i+1].next_turn = 0;
        bh_ptr[i+1] = binheap_insert(&queue, &objects[i+1]);
    }

    map_enemy_render(current);

    map_print(current);

    moveable* c;

    while (true) {
        c = (moveable*) binheap_remove_min(&queue);

        objects[c->order].next_turn += 100/c->speed;

        if (!c->is_player && current->enemies[c->order-1]->is_alive) {
            map_enemy_move(current, c->order-1);
            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        } else if (c->is_player) {
            bool valid_key = false;
            int ch = 0;
            while (!valid_key) {
                ch = getch();
                valid_key = true;
                switch (ch) {
                case 'm':
                case 'M':
                    map_display_enemies(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'i':
                    map_display_inventory(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'e':
                    map_display_equipment(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'I':
                    map_display_item_description(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'w':
                    map_wear_item(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 't':
                    map_take_off_item(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'd':
                    map_drop_item(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case 'x':
                    map_remove_item(current);

                    map_enemy_render(current);

                    map_print(current);
                    valid_key = false;
                    break;
                case '7':
                case 'y':
                case 'Y':
                case KEY_HOME:
                    map_player_move(current, -1, -1);
                    break;
                case '8':
                case 'k':
                case 'K':
                case KEY_UP:
                    map_player_move(current, 0, -1);
                    break;
                case '9':
                case 'u':
                case 'U':
                case KEY_PPAGE:
                    map_player_move(current, 1, -1);
                    break;
                case '4':
                case 'h':
                case 'H':
                case KEY_LEFT:
                    map_player_move(current, -1, 0);
                    break;
                case '5':
                case ' ':
                    // map_player_move(current, 0, 0);
                    break;
                case '6':
                case 'l':
                case 'L':
                case KEY_RIGHT:
                    map_player_move(current, 1, 0);
                    break;
                case '1':
                case 'b':
                case 'B':
                case KEY_END:
                    map_player_move(current, -1, 1);
                    break;
                case '2':
                case 'j':
                case 'J':
                case KEY_DOWN:
                    map_player_move(current, 0, 1);
                    break;
                case '3':
                case 'n':
                case 'N':
                case KEY_NPAGE:
                    map_player_move(current, 1, 1);
                    break;
                case '>':
                    printw("Going down...\n");
                    endwin();

                    binheap_delete(&queue);

                    free(objects);
                    free(bh_ptr);
                    return 1;
                    break;
                case '<':
                    printw("Going up...\n");
                    getch();
                    endwin();

                    binheap_delete(&queue);

                    free(objects);
                    free(bh_ptr);
                    return 2;
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

                    return -1;
                    break;
                default:
                    valid_key = false;
                }

                map_enemy_render(current);

                map_print(current);
            }

            map_enemy_render(current);

            map_print(current);

            bh_ptr[c->order] = binheap_insert(&queue, &objects[c->order]);
        }

        if (current->main_character->hp <= 0) {
            clear();
            attron(COLOR_PAIR(COLOR_BLACK+1));
            printw("*-----------------------------------------------------------------------------*\n");
            printw("|                                                                             |\n");
            printw("|                          _______________________                            |\n");
            printw("|                         /                   \\###\\                           |\n");
            printw("|                        |=====================|###|                          |\n");
            printw("|                        |                     |###|                          |\n");
            printw("|                        | Requiescat in pace  |###|                          |\n");
            printw("|                        |                     |###|                          |\n");
            printw("|                        | Main '@' Character  |###|                          |\n");
            printw("|                        | 'Die doing what you |###|                          |\n");
            printw("|                        |   love' ~ unknown   |###|                          |\n");
            printw("|                        |                     |###|                          |\n");
            printw("|         \\|/            |  Succeeded by:      |###|           \\|/            |\n");
            printw("|        --O--           |   npc1  npc3  npc5  |###|          --O--           |\n");
            printw("|         /|\\            |   npc2  npc4  npc6  |###|           /|\\            |\n");
            printw("|          |             |                     |###|            |             |\n");
            printw("|        |\\|             |  And a hoard of     |###|          |\\|             |\n");
            printw("|         \\|/|           |  little monsters :O |###|           \\|/|           |\n");
            printw("|          |/            |_____________________|###|            |/            |\n");
            printw("|wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww|\n");
            printw("|wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww|\n");
            printw("|wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww|\n");
            printw("|wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww|\n");
            printw("*-----------------------------------------------------------------------------*\n");
            attroff(COLOR_PAIR(COLOR_BLACK+1));
            break;
        } else {
            bool has_enemies = false;;
            for (int i = 0; i < current->enemy_count; i++) {
                if (current->enemies[i]->hp > 0) {
                    has_enemies = true;
                } else {
                    current->enemies[i]->is_alive = false;
                }
            }

            if (!has_enemies) {
                clear();
                attron(COLOR_PAIR(COLOR_YELLOW+1));
                printw("*-----------------------------------------------------------------------------*\n");
                printw("|                                                                             |\n");
                printw("|                                                                             |\n");
                printw("|                                       *                                     |\n");
                printw("|                      *               ***               *                    |\n");
                printw("|                     ***             *****             ***                   |\n");
                printw("|               *    *****     *     *******     *     *****    *             |\n");
                printw("|               *   *******   ***   *********   ***   *******   *             |\n");
                printw("|               *************************************************             |\n");
                printw("|               *************************************************             |\n");
                printw("|               =================================================             |\n");
                printw("|                    |_      |_    |_|_    |_|_|_  |_     |_                  |\n");
                printw("|                    |_|_  |_|_  |_    |_    |_    |_|_   |_                  |\n");
                printw("|                    |_  |_  |_  |_|_|_|_    |_    |_ |_  |_                  |\n");
                printw("|                    |_      |_  |_    |_    |_    |_   |_|_                  |\n");
                printw("|                    |_      |_  |_    |_  |_|_|_  |_     |_                  |\n");
                printw("|                                                                             |\n");
                printw("|                                                                             |\n");
                printw("|                The dungeons have been conquered, Oh Mighty King!            |\n");
                printw("|                                                                             |\n");
                printw("|                     All Hail King Main! All Hail King Main!                 |\n");
                printw("|                                                                             |\n");
                printw("|                                                                             |\n");
                printw("*-----------------------------------------------------------------------------*\n");
                attroff(COLOR_PAIR(COLOR_YELLOW+1));
                break;
            }
        }

        map_print(current);
    }

    getch();
    endwin();

    binheap_delete(&queue);

    free(objects);
    free(bh_ptr);
    return 0;
}

void map_display_enemies(map_c* current)
{
    WINDOW *subwin;
    int offset = 0;
    subwin = newpad(current->enemy_count, 50);
    for (int i = 0; i < current->enemy_count; i++) {
        int dx = current->enemies[i]->pos_x - current->main_character->pos_x;
        char* upordown = (char*) "right";
        if (dx < 0) {
            upordown = (char*) "left";
            dx *= -1;
        }

        int dy = current->enemies[i]->pos_y - current->main_character->pos_y;
        char* leftorright = (char*) "down";
        if (dy < 0) {
            leftorright = (char*) "up";
            dy *= -1;
        }
        char* aliveordead = (char*) "dead";

        if (current->enemies[i]->is_alive ) {
            aliveordead = (char*) "alive";
        }

        wprintw(subwin, "%c [%s] - pos: %i %s, %i %s: speed: %i\n", current->enemies[i]->representation, aliveordead, dx, upordown, dy, leftorright, current->enemies[i]->speed);
    }
    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, offset, 0, (LINES-15)/2, (COLS-50)/2, (LINES+15)/2, (COLS+50)/2);
    doupdate();

    int ch = 0;
    while (true) {
        ch = getch();
        switch (ch) {
        case KEY_UP:
            offset -= 1;
            if (offset >= current->enemy_count - 16) {
                offset = current->enemy_count - 16;
            }
            if (offset < 0) {
                offset = 0;
            }
            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, offset, 0, (LINES-15)/2, (COLS-50)/2, (LINES+15)/2, (COLS+50)/2);
            doupdate();
            break;
        case KEY_DOWN:
            offset += 1;
            if (offset >= current->enemy_count - 16) {
                offset = current->enemy_count - 16;
            }
            if (offset < 0) {
                offset = 0;
            }
            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, offset, 0, (LINES-15)/2, (COLS-50)/2, (LINES+15)/2, (COLS+50)/2);
            doupdate();
            break;
        case 27:
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        }
    }

    delwin(subwin);
}

void map_wear_item(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(24, 80);

    wprintw(subwin, "What item would you like to wear?\nUse 0-9 to select item.\n");

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
    doupdate();

    bool has_item = false;
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 27 || has_item) {
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        } else if (!has_item && (ch - 'a' >= 0) && (ch - 'l' <= 0)) {
            wprintw(subwin, "We will decide where it goes, thank you very much.\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        } else if (!has_item && (ch - '0' >= 0) && (ch - '9' <= 0)) {
            int i = ch - '0';
            object_t* inventory_2 = new object_t();

            if (i >= current->main_character->inventory->stack_size) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot is empty. Try again.\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                delete inventory_2;
                continue;
            }

            has_item = true;

            inventory_2->display = false;
            inventory_2->name = "Player Inventory";
            inventory_2->description = "This is the stack containing the player's inventory.\n";
            inventory_2->type = "stack";
            inventory_2->representation = '&';
            inventory_2->no_recursive = true;

            while (current->main_character->inventory->stack_size > i) {
                inventory_2->add_to_stack(current->main_character->inventory->pick_from_top_of_stack());
            }

            if (inventory_2->stack_size == 0) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot has an error. Try again?\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                delete inventory_2;
                continue;
            }

            object_t* item_from_inventory = inventory_2->pick_from_top_of_stack();
            object_t* swap = NULL;

            if (item_from_inventory->type == "WEAPON") {
                swap = current->main_character->equipment[0];
                current->main_character->equipment[0] = item_from_inventory;
            } else if (item_from_inventory->type == "OFFHAND") {
                swap = current->main_character->equipment[1];
                current->main_character->equipment[1] = item_from_inventory;
            } else if (item_from_inventory->type == "RANGED") {
                swap = current->main_character->equipment[2];
                current->main_character->equipment[2] = item_from_inventory;
            } else if (item_from_inventory->type == "ARMOR") {
                swap = current->main_character->equipment[3];
                current->main_character->equipment[3] = item_from_inventory;
            } else if (item_from_inventory->type == "HELMET") {
                swap = current->main_character->equipment[4];
                current->main_character->equipment[4] = item_from_inventory;
            } else if (item_from_inventory->type == "CLOAK") {
                swap = current->main_character->equipment[5];
                current->main_character->equipment[5] = item_from_inventory;
            } else if (item_from_inventory->type == "GLOVES") {
                swap = current->main_character->equipment[6];
                current->main_character->equipment[6] = item_from_inventory;
            } else if (item_from_inventory->type == "BOOTS") {
                swap = current->main_character->equipment[7];
                current->main_character->equipment[7] = item_from_inventory;
            } else if (item_from_inventory->type == "AMULET") {
                swap = current->main_character->equipment[8];
                current->main_character->equipment[8] = item_from_inventory;
            } else if (item_from_inventory->type == "LIGHT") {
                swap = current->main_character->equipment[9];
                current->main_character->equipment[9] = item_from_inventory;
            } else if (item_from_inventory->type == "RING") {
                if (current->main_character->equipment[10] == NULL) {
                    current->main_character->equipment[10] = item_from_inventory;
                } else {
                    swap = current->main_character->equipment[11];
                    current->main_character->equipment[11] = item_from_inventory;
                }
            }

            if (swap != NULL) {
                current->main_character->inventory->add_to_stack(swap);
            }

            while (inventory_2->stack_size > 0) {
                object_t* tmp = inventory_2->pick_from_top_of_stack();
                if (tmp->representation != '&') {
                    current->main_character->inventory->add_to_stack(tmp);
                }
            }

            delete inventory_2;

            wprintw(subwin, "Equiped item!\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        }
    }
}

void map_drop_item(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(24, 80);

    wprintw(subwin, "What item would you like to drop?\nUse 0-9 to select item.\n");

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
    doupdate();

    bool has_item = false;
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 27 || has_item) {
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        } else if (!has_item && (ch - 'a' >= 0) && (ch - 'l' <= 0)) {
            wprintw(subwin, "Please take off items before dropping them.\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        } else if (!has_item && (ch - '0' >= 0) && (ch - '9' <= 0)) {
            int i = ch - '0';
            object_t* inventory_2 = new object_t();

            if (i >= current->main_character->inventory->stack_size) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot is empty. Try again.\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                delete inventory_2;
                continue;
            }

            has_item = true;

            inventory_2->display = false;
            inventory_2->name = "Player Inventory";
            inventory_2->description = "This is the stack containing the player's inventory.\n";
            inventory_2->type = "stack";
            inventory_2->representation = '&';
            inventory_2->no_recursive = true;

            while (current->main_character->inventory->stack_size > i) {
                inventory_2->add_to_stack(current->main_character->inventory->pick_from_top_of_stack());
            }

            if (inventory_2->stack_size == 0) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot has an error. Try again?\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                delete inventory_2;
                continue;
            }

            object_t* item_from_inventory = inventory_2->pick_from_top_of_stack();

            item_from_inventory->display = true;
            item_from_inventory->pos_x = current->main_character->pos_x;
            item_from_inventory->pos_y = current->main_character->pos_y;

            if (current->objects_location[current->main_character->pos_y][current->main_character->pos_x] == NULL) {
                current->objects_location[current->main_character->pos_y][current->main_character->pos_x] = item_from_inventory;
            } else {
                item_from_inventory->pos_x = current->main_character->pos_x;
                item_from_inventory->pos_y = current->main_character->pos_y;
                current->objects_location[current->main_character->pos_y][current->main_character->pos_x]->add_to_stack(item_from_inventory);
            }

            while (inventory_2->stack_size > 0) {
                object_t* tmp = inventory_2->pick_from_top_of_stack();
                if (tmp->representation != '&') {
                    current->main_character->inventory->add_to_stack(tmp);
                }
            }

            delete inventory_2;

            wprintw(subwin, "Dropped item!\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        }
    }
}

void map_remove_item(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(24, 80);

    wprintw(subwin, "What item would you like to remove?\nUse 0-9 to select item.\n");

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
    doupdate();

    bool has_item = false;
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 27 || has_item) {
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        } else if (!has_item && (ch - 'a' >= 0) && (ch - 'l' <= 0)) {
            wprintw(subwin, "Please take off items before removing them.\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        } else if (!has_item && (ch - '0' >= 0) && (ch - '9' <= 0)) {
            int i = ch - '0';
            object_t* inventory_2 = new object_t();

            if (i >= current->main_character->inventory->stack_size) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot is empty. Try again.\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                delete inventory_2;
                continue;
            }

            has_item = true;

            inventory_2->display = false;
            inventory_2->name = "Player Inventory";
            inventory_2->description = "This is the stack containing the player's inventory.\n";
            inventory_2->type = "stack";
            inventory_2->representation = '&';
            inventory_2->no_recursive = true;

            while (current->main_character->inventory->stack_size > i) {
                inventory_2->add_to_stack(current->main_character->inventory->pick_from_top_of_stack());
            }

            if (inventory_2->stack_size == 0) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot has an error. Try again?\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
            }

            inventory_2->pick_from_top_of_stack();

            while (inventory_2->stack_size > 0) {
                object_t* tmp = inventory_2->pick_from_top_of_stack();
                if (tmp->representation != '&') {
                    current->main_character->inventory->add_to_stack(tmp);
                }
            }

            delete inventory_2;

            wprintw(subwin, "Removed item!\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        }
    }
}

void map_take_off_item(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(24, 80);

    wprintw(subwin, "What item would you like to take off?\nUse a-l to select equipment slot.\n");

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
    doupdate();

    bool has_item = false;
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 27 || has_item) {
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        } else if (!has_item && (ch - 'a' >= 0) && (ch - 'l' <= 0)) {
            int i = ch - 'a';

            if (current->main_character->equipment[i] == NULL) {
                has_item = false;
                wprintw(subwin, "Hmm, that slot is empty. Try again.\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                continue;
            }

            has_item = true;

            if (current->main_character->inventory->stack_size >= 10) {
                wprintw(subwin, "Hmm, not enough slots in inventory. Try again after discarding some items.\n");

                wnoutrefresh(stdscr);
                pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
                doupdate();
                continue;
            }

            current->main_character->inventory->add_to_stack(current->main_character->equipment[i]);
            current->main_character->equipment[i] = NULL;

            wprintw(subwin, "Took off item!\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        } else if (!has_item && (ch - '0' >= 0) && (ch - '9' <= 0)) {
            wprintw(subwin, "We will decide where it goes, thank you very much.\n");

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();
        }
    }
}

void map_display_item_description(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(24, 80);

    wprintw(subwin, "What item would you like to display the description of?\nUse 0-9 for inventory, a-l for equipment.\n");

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
    doupdate();

    bool has_description = false;
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 27 || has_description) {
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        } else if (!has_description && (ch - 'a' >= 0) && (ch - 'l' <= 0)) {
            int i = ch - 'a';

            if (current->main_character->equipment[i] != NULL) {
                wprintw(subwin, "[%i:%s] - %s: %s - %s\n", i, player_equipment_slot_names[i], current->main_character->equipment[i]->representation, current->main_character->equipment[i]->name.c_str(), current->main_character->equipment[i]->type.c_str());
                wprintw(subwin, "%s", current->main_character->equipment[i]->description.c_str());
            } else {
                wprintw(subwin, "[%i:%s] - <EMPTY>\n", i, player_equipment_slot_names[i]);
            }

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 0, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();

            has_description = true;
        } else if (!has_description && (ch - '0' >= 0) && (ch - '9' <= 0)) {
            int i = ch - '0';
            if (i < current->main_character->inventory->stack_size) {
                wprintw(subwin, "[%i] - %c: %s - %s\n", i, current->main_character->inventory->stack[i]->representation, current->main_character->inventory->stack[i]->name.c_str(), current->main_character->inventory->stack[i]->type.c_str());
                wprintw(subwin, "%s", current->main_character->inventory->stack[i]->description.c_str());
            } else {
                wprintw(subwin, "[%i] - <EMPTY>\n", i, player_equipment_slot_names[i]);
            }

            wnoutrefresh(stdscr);
            pnoutrefresh(subwin, 2, 0, (LINES-24)/2, (COLS-80)/2, (LINES+24)/2, (COLS+80)/2);
            doupdate();

            has_description = true;
        }
    }

    delwin(subwin);
    doupdate();
    refresh();
    clear();
}

void map_display_equipment(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(30, 50);

    for (int i = 0; i < 12; i++) {
        if (current->main_character->equipment[i] != NULL) {
            wprintw(subwin, "[%i:%s]: %s - %s\n", i, player_equipment_slot_names[i], current->main_character->equipment[i]->name.c_str(), current->main_character->equipment[i]->type.c_str());
        } else {
            wprintw(subwin, "[%i:%s] - <EMPTY>\n", i, player_equipment_slot_names[i]);
        }
    }

    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-12)/2, (COLS-50)/2, (LINES+12)/2, (COLS+50)/2);
    doupdate();

    int ch = 0;
    while (true) {
        ch = getch();
        switch (ch) {
        case 27:
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        }
    }

    delwin(subwin);
    doupdate();
    refresh();
    clear();
}


void map_display_inventory(map_c* current)
{
    WINDOW *subwin;
    subwin = newpad(30, 50);
    for (int i = 0; i < current->main_character->inventory->stack_size; i++) {
        wprintw(subwin, "[%i] - %c: %s - %s\n", i, current->main_character->inventory->stack[i]->representation, current->main_character->inventory->stack[i]->name.c_str(), current->main_character->inventory->stack[i]->type.c_str());
    }
    for (int i = current->main_character->inventory->stack_size; i < 10; i++) {
        wprintw(subwin, "[%i] - <empty>\n", i);
    }
    wnoutrefresh(stdscr);
    pnoutrefresh(subwin, 0, 0, (LINES-10)/2, (COLS-50)/2, (LINES+10)/2, (COLS+50)/2);
    doupdate();

    int ch = 0;
    while (true) {
        ch = getch();
        switch (ch) {
        case 27:
            delwin(subwin);
            doupdate();
            refresh();
            clear();
            return;
        }
    }

    delwin(subwin);
    doupdate();
    refresh();
    clear();
}

void map_render_splash()
{

    initscr();
    int row, col;
    getmaxyx(stdscr,row,col);
    if (col < 79 || row < 23) {
        return;
    }

    if (has_colors() == FALSE) {
        return;
    }

    start_color();
    init_pair(COLOR_BLACK+1, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_RED+1, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN+1, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW+1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_MAGENTA+1, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(COLOR_BLUE+1, COLOR_BLUE, COLOR_WHITE);
    init_pair(COLOR_CYAN+1, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE+1, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(COLOR_BLACK+1));
    printw("*-----------------------------------------------------------------------------*\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    attroff(COLOR_PAIR(COLOR_BLACK)+1);
    attron(COLOR_PAIR(COLOR_RED+1));
    printw("|                                                                             |\n");
    printw("|                           |_|_|_  |_|_|_  |_                                |\n");
    printw("|                           |_      |_      |_                                |\n");

    attroff(COLOR_PAIR(COLOR_RED+1));
    attron(COLOR_PAIR(COLOR_GREEN+1));
    printw("|                           |_      |_|_    |_                                |\n");
    printw("|                           |_      |_      |_                                |\n");
    attroff(COLOR_PAIR(COLOR_GREEN+1));
    attron(COLOR_PAIR(COLOR_YELLOW+1));
    printw("|                           |_|_|_  |_|_|_  |_|_|_                            |\n");
    printw("|                                                                             |\n");
    printw("|                           cipherboy~                                        |\n");
    attroff(COLOR_PAIR(COLOR_YELLOW+1));
    attron(COLOR_PAIR(COLOR_MAGENTA+1));
    printw("|                                entertainment                                |\n");
    printw("|                                          limited                            |\n");
    printw("|                                                                             |\n");
    attroff(COLOR_PAIR(COLOR_MAGENTA+1));
    attron(COLOR_PAIR(COLOR_BLUE+1));
    printw("|                           v1.0%i ~ \"LoP\"                                     |\n", 9);
    printw("|                                                                             |\n");
    printw("|                         Optimized for POWER7+/POWER 8                       |\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    printw("|                                                                             |\n");
    attroff(COLOR_PAIR(COLOR_BLUE+1));
    attron(COLOR_PAIR(COLOR_CYAN+1));
    printw("|                                                     COPYRIGHT (C) 2016      |\n");
    printw("|                                                     ALL RIGHTS RESERVED     |\n");
    attroff(COLOR_PAIR(COLOR_CYAN+1));
    attron(COLOR_PAIR(COLOR_WHITE+1));
    printw("|                                                     (see LICENSE for terms) |\n");
    printw("*-----------------------------------------------------------------------------*\n");
    attroff(COLOR_PAIR(COLOR_WHITE+1));
    refresh();
    sleep(1);
    endwin();
}
