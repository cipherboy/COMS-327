# Project 1.06

    u: Alexander Scheel
    e: <scheel@iastate.edu>
    l: BSD: 2-clause

## Overview
This part of the project deals with interfacing between C and C++. This
project also adds a fog of war element. The player has a 3x3 grid "lamp",
and anything outside of the lamp is hidden. However, new modifications
to the same grid will not be revisited. Monsters outside of the lamp are
similarly invisible.

Keybindings remain the same from the previous version of the project:

     ---------- ---------- ---------------
    | 7/y/home |  8/k/up  |  9/u/page up  |
    |----------|----------|---------------|
    | 4/h/left |  5/space |   6/l/right   |
    |----------|----------|---------------|
    | 1/b/end  | 2/j/down | 3/n/page down |
     ---------- ---------- ---------------

This ensures that the keypad works with and without number lock on.

To show the menu, press m; to go down or up, press > or <; and
to quit (no save!), press s.

## Running
To compile and run:  

    make all
    ./bin/rlg327
