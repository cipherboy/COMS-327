/**
 * Copyright 2016 Alexander Scheel
 *
 * Character header file
**/

#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "map.h"

class character_t
{
protected:
  map_t* current;

public:
  int pos_x;
  int pos_y;

  bool is_alive;

  char representation;

  int speed;

  character_t(map_t* current);
  ~character_t();
};

#endif
