/**
 * Copyright 2016 Alexander Scheel
 *
 * Map monster implementation file
**/

#include "map_item_reader.h"

#include "object.h"
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

object_t** map_item_parse_file(map_t* current, char* basepath)
{
    string path = basepath;
    path += "/object_desc.txt";

    ifstream f (path.c_str());
    if (!f) {
        cout << "Error reading file." << endl;
        return NULL;
    }

    object_t** results = (object_t**) malloc(sizeof(object_t*) * 1);

    string real_line;
    if (!getline(f, real_line)) {
        cout << "Error reading first line of file." << endl;
        return NULL;
    }

    if (real_line != "RLG327 OBJECT DESCRIPTION 1") {
        cout << "First line of file mismatch." << endl;
        return NULL;
    }

    int current_object = -1;
    bool in_description = false;
    bool in_monster = false;
    bool has_error = false;
    bool has_name = false;
    bool has_type = false;
    bool has_color = false;
    bool has_description = false;
    bool has_hit = false;
    bool has_damage = false;
    bool has_dodge = false;
    bool has_defense = false;
    bool has_weight = false;
    bool has_speed = false;
    bool has_attr = false;
    bool has_value = false;

    while (getline(f, real_line)) {
        string line;
        line.assign(real_line);
        if (line == "BEGIN OBJECT" || line == "begin object") {
            if (in_monster && !has_error) {
                cout << "Not keeping old object even though no error" << endl;
                delete results[current_object];
                current_object -= 1;
            } else if (has_error) {
                cout << "Discarding current object due to previous errors" << endl;
                delete results[current_object];
                current_object -= 1;
            }

            in_description = false;
            in_monster = true;
            has_error = false;
            has_name = false;
            has_color = false;
            has_type = false;
            has_description = false;
            has_speed = false;
            has_hit = false;
            has_damage = false;
            has_dodge = false;
            has_defense = false;
            has_weight = false;
            has_speed = false;
            has_attr = false;
            has_value = false;

            current_object += 1;
            results = (object_t **) realloc(results, sizeof(object_t*) * (current_object+2));
            results[current_object] = new object_t();
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
                results[current_object]->name = name;
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

            results[current_object]->description = "";
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
                results[current_object]->color = color;
            }
        }

        if (!has_error && in_monster && (line.substr(0, 4) == "TYPE" || line.substr(0, 4) == "type")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_type) {
                has_error = true;
                cout << "Duplicate TYPE token" << endl;
                continue;
            }

            has_type = true;

            string type = line.substr(5);
            if (type == "") {
                has_error = true;
                cout << "Error: missing TYPE value" << endl;
            } else {
                results[current_object]->type = type;
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
                results[current_object]->speed = dice_t();
                results[current_object]->speed.parse((char *) speed.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 3) == "HIT" || line.substr(0, 3) == "hit")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_hit) {
                has_error = true;
                cout << "Duplicate HIT token" << endl;
                continue;
            }

            has_hit = true;

            string hit = line.substr(4);
            if (hit == "") {
                has_error = true;
                cout << "Error: missing HIT value" << endl;
            } else {
                results[current_object]->hit = dice_t();
                results[current_object]->hit.parse((char *) hit.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 3) == "DAM" || line.substr(0, 3) == "dam")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_damage) {
                has_error = true;
                cout << "Duplicate DAM token" << endl;
                continue;
            }

            has_damage = true;

            string damage = line.substr(4);
            if (damage == "") {
                has_error = true;
                cout << "Error: missing DAM value" << endl;
            } else {
                results[current_object]->damage = dice_t();
                results[current_object]->damage.parse((char *) damage.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 5) == "DODGE" || line.substr(0, 5) == "dodge")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_dodge) {
                has_error = true;
                cout << "Duplicate DODGE token" << endl;
                continue;
            }

            has_dodge = true;

            string dodge = line.substr(6);
            if (dodge == "") {
                has_error = true;
                cout << "Error: missing DODGE value" << endl;
            } else {
                results[current_object]->dodge = dice_t();
                results[current_object]->dodge.parse((char *) dodge.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 3) == "DEF" || line.substr(0, 3) == "def")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_defense) {
                has_error = true;
                cout << "Duplicate DEF token" << endl;
                continue;
            }

            has_defense = true;

            string defense = line.substr(4);
            if (defense == "") {
                has_error = true;
                cout << "Error: missing DEF value" << endl;
            } else {
                results[current_object]->defense = dice_t();
                results[current_object]->defense.parse((char *) defense.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 6) == "WEIGHT" || line.substr(0, 6) == "weight")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_weight) {
                has_error = true;
                cout << "Duplicate WEIGHT token" << endl;
                continue;
            }

            has_weight = true;

            string weight = line.substr(7);
            if (weight == "") {
                has_error = true;
                cout << "Error: missing WEIGHT value" << endl;
            } else {
                results[current_object]->weight = dice_t();
                results[current_object]->weight.parse((char *) weight.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 4) == "ATTR" || line.substr(0, 4) == "attr")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_attr) {
                has_error = true;
                cout << "Duplicate ATTR token" << endl;
                continue;
            }

            has_attr = true;

            string attr = line.substr(6);
            if (attr == "") {
                has_error = true;
                cout << "Error: missing ATTR value" << endl;
            } else {
                results[current_object]->attr = dice_t();
                results[current_object]->attr.parse((char *) attr.c_str());
            }
        }

        if (!has_error && in_monster && (line.substr(0, 3) == "VAL" || line.substr(0, 3) == "val")) {
            if (in_description) {
                has_error = true;
                cout << "Error: description not terminated" << endl;
            }

            if (has_value) {
                has_error = true;
                cout << "Duplicate VAL token" << endl;
                continue;
            }

            has_value = true;

            string val = line.substr(4);
            if (val == "") {
                has_error = true;
                cout << "Error: missing VAL value" << endl;
            } else {
                results[current_object]->value = dice_t();
                results[current_object]->value.parse((char *) val.c_str());
            }
        }

        if (!has_error && in_description && line != "DESC" && line != "desc") {
            if (line == ".") {
                in_description = false;
                continue;
            }

            if (line.length() <= 77) {
                results[current_object]->description += line + "\n";
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

            if (!has_name || !has_color || !has_type || !has_description || !has_speed || !has_hit || !has_damage || !has_dodge || !has_defense || !has_weight || !has_speed || !has_attr || !has_value) {
                cout << "Error: missing required field." << endl;
                has_error = true;
            }
        }
    }

    if (in_monster && !has_error) {
        cout << "Not keeping last object even though no error" << endl;
        delete results[current_object];
        current_object -= 1;
    } else if (has_error) {
        cout << "Discarding last object due to previous errors" << endl;
        delete results[current_object];
        current_object -= 1;
    }

    cout << endl << endl << endl << "Parsed monster results:" << endl;

    for (int j = 0; j <= current_object; j++) {
        object_t m = *results[j];
        cout << m.name << endl;
        cout << m.description;
        cout << m.type << endl;
        cout << m.color << endl;
        cout << m.hit.print() << endl;
        cout << m.damage.print() << endl;
        cout << m.dodge.print() << endl;
        cout << m.defense.print() << endl;
        cout << m.weight.print() << endl;
        cout << m.speed.print() << endl;
        cout << m.attr.print() << endl;
        cout << m.value.print() << endl << endl;
        delete results[j];
    }

    f.close();

    free(results);

    return NULL;
}
