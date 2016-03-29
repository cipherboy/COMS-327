/**
 * Copyright 2016 Alexander Scheel
 *
 * Character header file
**/

#include "map.h"

#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "stdbool.h"

class character_t
{
protected:
  map* current;

public:
  int pos_x;
  int pos_y;

  bool is_alive;

  char representation;

  int speed;

  character_t(map* current);
  ~character_t();
};

#endif
