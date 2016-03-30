# Project 1.07

    u: Alexander Scheel
    e: <scheel@iastate.edu>
    l: BSD: 2-clause

## Overview
This part of the project finishes the conversion to C++. Since this project has
thus been fully converted, the few remaining structs have been converted to
classes. Finally, the development of an editable EDL, or Entity Description
Language for monsters and objects creates a modable interface to the game.

In addition, two new command line options were created:

    --monsters  :::  parse the monsters definition file
    --items     :::  parse the items definition file

If either of these two options are specified, the game will not run and instead
only the desired information will be displayed.

Note: implicit assumption that the files work with your C++ standard library's
getline() istream function.

## Running
To compile and run:  

    make all
    ./bin/rlg327
