/***************************************************************************
                          spashscreen.h  -  description
                             -------------------
    begin                : Seg Mar 3 2003
    copyright            : (C) 2003 by Alexander Kjäll
    email                : <capitol@sigma.su.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "includes.h"
#include "player.h"

class SplashScreen
{
public:
	SplashScreen() {}
	~SplashScreen() {}

	int showTitleScreen(SDL_Surface *screen);

private:
	int keyEvents();
};

#endif
