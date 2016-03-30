/**
 * Copyright 2016 Alexander Scheel
 *
 * Map monster implementation file
**/

#include "character.h"
#include "enemy.h"
#include "player.h"
#include "map.h"
#include "utils.h"
#include "dice.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

character_t** map_monster_parse_file(map_t* current, char* basepath)
{
    string path = basepath;
    path += "/monster_desc.txt";

    ifstream f (path.c_str());
    if (!f) {
        cout << "Error reading file." << endl;
        return NULL;
    }

    character_t** results = (character_t**) malloc(sizeof(character_t*) * 1);

    string real_line;
    if (!getline(f, real_line)) {
        cout << "Error reading first line of file." << endl;
        return NULL;
    }

    if (real_line != "RLG327 MONSTER DESCRIPTION 1") {
        cout << "First line of file mismatch." << endl;
        return NULL;
    }

    int current_monster = -1;
    bool in_description = false;
    bool in_monster = false;
    bool has_error = false;
    bool has_name = false;
    bool has_color = false;
    bool has_description = false;
    bool has_speed = false;
    bool has_abilities = false;
    bool has_hp = false;
    bool has_damages = false;

    while (getline(f, real_line)) {
        string line;
        line.assign(real_line);
        if (line == "BEGIN MONSTER" || line == "begin monster") {
            if (in_monster && !has_error) {
                cout << "Not keeping old monster even though no error" << endl;
                delete results[current_monster];
                current_monster -= 1;
            } else if (has_error) {
                cout << "Discarding current monster due to previous errors" << endl;
                delete results[current_monster];
                current_monster -= 1;
            }

            in_description = false;
            in_monster = true;
            has_error = false;
            has_name = false;
            has_color = false;
            has_description = false;
            has_speed = false;
            has_abilities = false;
            has_hp = false;
            has_damages = false;

            current_monster += 1;
            results = (character_t **) realloc(results, sizeof(character_t*) * (current_monster+2));
            results[current_monster] = new character_t(current);
        }

        if (!has_error && in_monster && (line.substr(0, 4) == "NAME" || line.substr(0, 4) == "name")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_name) {
                has_error = true;
                cout << "Duplicate NAME token" << endl;
                continue;
            }

            has_name = true;

            string name = line.substr(5);
            if (name == "") {
                has_error = true;
                cout << "Error: missing NAME value" << endl;
            } else {
                results[current_monster]->name = name;
            }
        }

        if (!has_error && in_monster && (line == "DESC" || line == "desc")) {
            in_description = true;

            if (has_description) {
                has_error = true;
                cout << "Duplicate DESC token" << endl;
                continue;
            }

            has_description = true;

            results[current_monster]->description = "";
        }

        if (!has_error && in_monster && (line.substr(0, 5) == "COLOR" || line.substr(0, 5) == "color")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_color) {
                has_error = true;
                cout << "Duplicate COLOR token" << endl;
                continue;
            }

            has_color = true;

            string color = line.substr(6);
            if (color == "" || (color != "RED" && color != "red" && color != "GREEN" && color != "green" && color != "BLUE" && color != "blue" && color != "CYAN" && color != "cyan" && color != "YELLOW" && color != "yellow" && color != "MAGENTA" && color != "magenta" && color != "WHITE" && color != "white" && color != "BLACK" && color != "black")) {
                has_error = true;
                cout << "Error: missing or invalid COLOR value: " << color << endl;
            } else {
                results[current_monster]->color = color;
            }
        }

        if (!has_error && in_monster && (line.substr(0, 5) == "SPEED" || line.substr(0, 5) == "speed")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_speed) {
                has_error = true;
                cout << "Duplicate SPEED token" << endl;
                continue;
            }

            has_speed = true;

            string speed = line.substr(5);
            if (speed == "") {
                has_error = true;
                cout << "Error: missing SPEED value" << endl;
            } else {
                results[current_monster]->speed_dice = dice_t();
                results[current_monster]->speed_dice.parse((char *) speed.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 4) == "ABIL" || line.substr(0, 4) == "abil")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_abilities) {
                has_error = true;
                cout << "Duplicate ABIL token" << endl;
                continue;
            }

            has_abilities = true;

            string abilities = line.substr(5);
            if (abilities == "") {
                has_error = true;
                cout << "Error: missing ABILITIES value" << endl;
            } else {
                results[current_monster]->abilities = abilities;
            }
        }

        if (!has_error && in_monster && (line.substr(0, 2) == "HP" || line.substr(0, 2) == "hp")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_hp) {
                has_error = true;
                cout << "Duplicate HP token" << endl;
                continue;
            }

            has_hp = true;

            string hp = line.substr(3);
            if (hp == "") {
                has_error = true;
                cout << "Error: missing HP value" << endl;
            } else {
                results[current_monster]->hp = dice_t();
                results[current_monster]->hp.parse((char *) hp.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 3) == "DAM" || line.substr(0, 3) == "dam")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_damages) {
                has_error = true;
                cout << "Duplicate DAM token" << endl;
                continue;
            }

            has_damages = true;

            string dam = line.substr(4);
            if (dam == "") {
                has_error = true;
                cout << "Error: missing DAM value" << endl;
            } else {
                results[current_monster]->attack_damage = dice_t();
                results[current_monster]->attack_damage.parse((char *) dam.c_str());
            }
        }

        if (!has_error && in_description && line != "DESC" && line != "desc") {
            if (line == ".") {
                in_description = false;
                continue;
            }

            if (line.length() <= 77) {
                results[current_monster]->description += line + "\n";
            } else {
                has_error = true;
                cout << "Too long of line or missing newline in description" << endl;
            }
        }

        if (!has_error && (line.substr(0, 3) == "END" || line.substr(0, 3) == "end")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            in_monster = false;

            if (!has_name || !has_color || !has_description || !has_speed || !has_abilities || !has_hp || !has_damages) {
                cout << "Error: missing required field." << endl;
                has_error = true;
            }
        }
    }

    if (in_monster && !has_error) {
        cout << "Not keeping last monster even though no error" << endl;
        delete results[current_monster];
        current_monster -= 1;
    } else if (has_error) {
        cout << "Discarding last monster due to previous errors" << endl;
        delete results[current_monster];
        current_monster -= 1;
    }

    cout << endl << endl << endl << "Parsed monster results:" << endl;

    for (int j = 0; j <= current_monster; j++) {
        character_t m = *results[j];
        cout << m.name << endl;
        cout << m.description;
        cout << m.color << endl;
        cout << m.speed_dice.print() << endl;
        cout << m.abilities << endl;
        cout << m.hp.print() << endl;
        cout << m.attack_damage.print() << endl << endl;
        delete results[j];
    }

    f.close();

    free(results);

    return NULL;
}
