/***************************************************************************
                          includes.h  -  inclutions and definitions
                             -------------------
    begin                : Sáb Fev 8 2003
    copyright            : (C) 2003 by Ricardo Cruz
    email                : <rick2@aeiou.pt>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// libraries inclusions
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
// this includes are necessary for gcc < 3.1
#include <string.h>
#include <stdlib.h>

/* game definitions */
// video definitions
#define SCREENWIDTH  208
#define SCREENHEIGHT 224

// level definitions
#define TOTROWS 27  //(224/16)
#define TOTCOLS 26  // (208/16)

// memory values
#define MAXBOBBLES 4
#define MAXENEMIES 10

// directions
#define X 0
#define Y 1
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define SHOOT 4

#define LSHOOT 2
#define RSHOOT 3

// menu stuff
#define RETURN 5
#define QUIT 6
#define PREVLEVEL 7
#define NEXTLEVEL 8
#define ONEPLAYER 1
#define TWOPLAYER 2

// objects types
#define PLAYER 0
#define ENEMY 1
#define BOBBLE 2

// player's back tile
#define COL 0
#define ROW 1
#define FIRST 0
#define LAST 1

// velocity and timing definitons
#define NORMALDELAY 42

// blinking (when player losts or wins)
#define BLINK_DELAY 400   // used until game end or level change
#define NB_OF_BLINKS 5

// player
#define HORVEL 2.6
#define VERVEL 3
#define JUMPSPEEDTIME 0.5
#define JUMPAIRTIME 0.25
// bobble
#define BOBVERVEL 1.4		// velocity of bobbles
#define BOBHORVEL 3.5
#define BOBSHOOTTIME 0.7
#define BOBDESTROYTIME 3.5
// enemy
#define ENEHORVEL 2.6
#define ENEVERVEL 3

// images
#define REDCLRKEY 255
#define GREENCLRKEY 0
#define BLUECLRKEY 255

// graphical effect
#define HORENTERPIX 3		// number of pixels it "enters" on the left and right blocks
#define CHGFRAMETIME 0.4
// 'action' images
#define ACTDEFAULT 0
#define ACTENEMY1 1

#define SHOOT_IMG_TIME 400  // in ms
