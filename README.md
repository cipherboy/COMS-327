# Project 1.04

    u: Alexander Scheel
    e: <scheel@iastate.edu>
    l: BSD: 2-clause

## Overview
The goal of this part of the project is to add moving monsters and playing
characters to the game. In particular, there exist four base attributes any
monster could have:

- Intelligence
- Telepathy
- Tunneling
- Erratic Behavior

The probability of having any one of these attributes is 50%. At this stage,
the player's movement is unable to be controlled and simply wanders around
aimlessly. Poor guy, almost certain to die a horrible death...

...or that is what they would like you to believe. In reality, he kills any
monsters which get close to him, assuming they don't gang up and corner him.


## Running
To compile and run:  

    make all
    ./bin/rlg327



To date, longest running game:

    *--------------------------------------------------------------------------------*
    |                                                                                |
    |      ##    # #######  # #### ####           ### # ## ####  ################### |
    |      ###  ########### # #####  ##            #...... ######################### |
    |      .........############# #  ##        ## ##......########################## |
    |     #.........############..... #         ####......########################## |
    |     #.........#########   .....##         ####......########################## |
    |      .........####### #   .....      ##  #####......########################## |
    |    ##.........#######     .....  #    ########   ########################### # |
    |     #.........#######    #.....## #  #####################################     |
    |     ################## ##  ######## #    #################..............###    |
    |       ###### ############## ####### ##  ##################..............##     |
    |     ###################################################d##..............#      |
    |     ##############################  ##  ......## #######@#..............#      |
    |    ######### ####################    ## ......#### #######..............#      |
    |    ......######.........#######     #  #......### ####### ############         |
    | #  ......######.........#### #     #####...... #########   ###########         |
    | ###......######.........################......###################### #         |
    | ###......######.........######### ######...... # ###        #######            |
    | ###......######......... ######## ######......    #        ### ###             |
    | ############## ......... ###   ## ####   ### #                 ####            |
    |                                                                                |
    *--------------------------------------------------------------------------------*

    Main character won!
    Enemies freed~

    ==23408== HEAP SUMMARY:
    ==23408==     in use at exit: 0 bytes in 0 blocks
    ==23408==   total heap usage: 5,199,261 allocs, 5,199,261 frees, 462,508,146 bytes allocated
    ==23408==
    ==23408== All heap blocks were freed -- no leaks are possible

However, the program never exceeds 8K of memory useage! (realloc may or
may not be abused).
