/***************************************************************************
                          gamemanager.cpp  -  description
                             -------------------
    begin                : Sáb Fev 8 2003
    copyright            : (C) 2003 by Ricardo Cruz & Alexander Kjäll
    email                : <rick2@aeiou.pt> & <capitol@sigma.su.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "includes.h"
#include "menu.h"
#include "gamemanager.h"
#include "gameloop.h"

void runGame(SDL_Surface *screen)
{
SplashScreen *splash = new SplashScreen();
GameLoop *gameloop = new GameLoop();

int menuret, gameret;
while(true)
	{
	menuret = splash->showTitleScreen(screen);

	if(menuret == QUIT)
		break;

	for(int level = 1;; level++)
		{
		gameret = gameloop->runLevel(screen, "data/levels", level, menuret);

		if(level >= gameloop->total_levels)
			break;
    if(gameret == PREVLEVEL)
      {
      level -= 2;
      if(level < 0)
        gameret = RETURN;
      }
		if(gameret == QUIT || gameret == RETURN)
			break;

// 		else if(returnValue == RESTART)
// 			--i;
		}
	}

delete splash;
delete gameloop;
}
