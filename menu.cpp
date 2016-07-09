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

#include "includes.h"
#include "menu.h"
 
int SplashScreen::showTitleScreen(SDL_Surface *screen)
{
SDL_Surface *backgroundImage;
SDL_Rect backgroundDest;

int screendone = 0;
int event;
int dragonYPos[3] = {150, 170, 190};
int dragonPos = 0;
Player dragon("data/smalltux2.bmp", 100, 30, RIGHT, 4, 3);

/* Load the BMP file into a surface */
backgroundImage = SDL_LoadBMP("data/title.bmp");
if(backgroundImage == NULL)
	{
	cerr << "Couldn't load " << "data/title.bmp" << SDL_GetError();
	return 0;
	}
backgroundDest.x = 0;
backgroundDest.y = 0;
backgroundDest.w = backgroundImage->w;
backgroundDest.h = backgroundImage->h;

while(screendone == 0)
	{
	event = keyEvents();
	switch(event)
		{
		case UP:
			dragonPos = ((dragonPos - 1) % 3 + 3) % 3;
			break;
		case DOWN:
			dragonPos = ((dragonPos + 1) % 3 + 3) % 3;
			break;
		case RETURN:
			switch(dragonPos)
				{
				case 0:
					return ONEPLAYER;
					break;
				case 1:
					return TWOPLAYER;
					break;
				case 2:
					return QUIT;
					break;
				}
			case QUIT:
				return QUIT;
				break;
			}

  /* Blit onto the screen surface.
   The surfaces should not be locked at this point. */
	SDL_BlitSurface(backgroundImage, NULL, screen, &backgroundDest);

	dragon.setPos(dragonYPos[dragonPos], Y);
	dragon.setPos(40, X);
	dragon.draw(screen, 0);

	/* Update the changed portion of the screen */
	SDL_UpdateRects(screen, 1, &backgroundDest);
	}

SDL_FreeSurface(backgroundImage);
return 0;
}


int SplashScreen::keyEvents()
{
SDL_Event event;

/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
/* more events on the event queue, our while loop will exit when */
/* that occurs.                                                  */
while(SDL_PollEvent(&event))
	{
	// testing SDL_KEYDOWN, SDL_KEYUP and SDL_QUIT events
	switch(event.type)
		{
		case SDL_KEYDOWN:	// key pressed
			switch(event.key.keysym.sym)
				{
				case SDLK_UP:
					return UP;
					break;
				case SDLK_DOWN:
					return DOWN;
					break;
				case SDLK_RETURN:
					return RETURN;
					break;
				case SDLK_ESCAPE:
					return QUIT;
					break;
				default:
					break;
				}
			break;
		case SDL_QUIT:	// window closed
			return QUIT;
			break;
		default:
			break;
		}
	}
return 0;
}
