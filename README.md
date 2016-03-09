# Project 1.05

    u: Alexander Scheel
    e: <scheel@iastate.edu>
    l: BSD: 2-clause

## Overview
The goal of this part of the project is to add ncurses and player movement to
our projects. In general, this was accomplished using a single main window
with a pad for the enemy scrolling list.

The keybindings for movement are as follows:
    
     ---------- ---------- ---------------
    | 7/y/home |  8/k/up  |  9/u/page up  |
    |----------|----------|---------------|
    | 4/h/left |  5/space |   6/l/right   |
    |----------|----------|---------------|
    | 1/b/end  | 2/j/down | 3/n/page down |
     ---------- ---------- --------------- 

This ensures that the keypad works with and without numlock on.

To show the menu, press m; to go down or up, press > or <; and
to quit (no save!), press s. 

Enjoy the game, and may the odds be ever in your favor!


## Running
To compile and run:  

    make all
    ./bin/rlg327
