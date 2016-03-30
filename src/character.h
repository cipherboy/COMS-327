/**
 * Copyright 2016 Alexander Scheel
 *
 * Character header file
**/

#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "map.h"
#include "dice.h"
#include <string>

using namespace std;

class character_t
{
protected:
  map_t* current;

public:
  int pos_x;
  int pos_y;

  int hp;
  int attack_damage;

  bool is_alive;

  char representation;

  string name;
  string description;
  string color;
  string abilities;

  int speed;

  character_t(map_t* current);
  ~character_t();
};

#endif
