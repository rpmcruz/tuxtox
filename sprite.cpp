/***************************************************************************
                          sprite.cpp  -  description
                             -------------------
    begin                : Qui Fev 27 2003
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

#include "includes.h"
#include "sprite.h"

Sprite::Sprite(char *filename, int x, int y, int curAction, int totActions, int totFrames)
{
s_sprite = SDL_LoadBMP(filename);
SDL_SetColorKey(s_sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(s_sprite->format, REDCLRKEY, GREENCLRKEY, BLUECLRKEY));
if (s_sprite == NULL)
	cerr << "Couldn't open sprite file: " << filename << endl;

// SDL_SetAlpha(s_sprite, SDL_RLEACCEL | SDL_SRCALPHA, 180);

f_oldPos[X] = f_pos[X] = x;
f_oldPos[Y] = f_pos[Y] = y;
f_vel[X] = 0;
f_vel[Y] = 0;

i_curFrame = 0;
f_chgFrameTimer = 0;
i_totFrames = totFrames;
i_curAction = curAction;
i_totActions = totActions;

for(int i = 0; i < 4; i++)
	b_direction[i] = false;

for(int i = 0; i < 4; i++)
	b_colliding[i] = false;
}

Sprite::~Sprite()
{
SDL_FreeSurface(s_sprite);
}

SDL_Surface* Sprite::surface()
{
return s_sprite;
}

int Sprite::oldPos(int dir)
{
return (int)f_oldPos[dir];
}

int Sprite::pos(int dir)
{
return (int)f_pos[dir];
}

int Sprite::width()
{
return s_sprite->w/i_totFrames;
}

int Sprite::height()
{
return s_sprite->h/i_totActions;
}

void Sprite::setPos(int pos, int dir)
{
f_pos[dir] = pos;
}

void Sprite::setColliding(int dir, bool colliding)
{
b_colliding[dir] = colliding;
}

bool Sprite::colliding(int dir)
{
return b_colliding[dir];
}

void Sprite::setDirection(int direction, bool moving)
{
b_direction[direction] = moving;

if(direction == LEFT && moving == true)
	b_direction[RIGHT] = false;
if(direction == RIGHT && moving == true)
	b_direction[LEFT] = false;
if(direction == UP && moving == true)
	b_direction[DOWN] = false;
if(direction == DOWN && moving == true)
	b_direction[UP] = false;
}

bool Sprite::direction(int dir)
{
if(dir == LEFT && f_pos[X] < f_oldPos[X])
	return true;
if(dir == RIGHT && f_pos[X] > f_oldPos[X])
	return true;
if(dir == UP && f_pos[Y] < f_oldPos[Y])
	return true;
if(dir == DOWN && f_pos[Y] > f_oldPos[Y])
	return true;

return false;
}

void Sprite::setAlive(bool alive)
{
b_alive = alive;
}

bool Sprite::alive()
{
return b_alive;
}

void Sprite::setAction(int action)
{
if(action == SHOOT && i_curAction == LEFT)
	i_curAction = LSHOOT;
else if(action == SHOOT && i_curAction == RIGHT)
	i_curAction = RSHOOT;
else
	i_curAction = action;
}

void Sprite::updatePos(float f_diffTicks)
{
if(b_alive == true)
	{
	f_oldPos[X] = f_pos[X];
	f_oldPos[Y] = f_pos[Y];

	// see if there is any moviment
	if(b_direction[LEFT] == true)
		f_pos[X] -= f_vel[X] * f_diffTicks;

	if(b_direction[RIGHT] == true)
		f_pos[X] += f_vel[X] * f_diffTicks;

	if(b_direction[UP] == true)
		f_pos[Y] -= f_vel[Y] * f_diffTicks;

	if(b_direction[DOWN] == true)
		f_pos[Y] += f_vel[Y] * f_diffTicks;

	for(int dir = 0; dir < 4; dir++)
		b_colliding[dir] = false;

	// see if there is need to change frame
	if(SDL_GetTicks() - f_chgFrameTimer > CHGFRAMETIME*1000)
		{
		i_curFrame++;
		if(i_curFrame == i_totFrames)
			i_curFrame = 0;
		f_chgFrameTimer = SDL_GetTicks();
		}
	}
}

void Sprite::draw(SDL_Surface *screen, Uint32 color)
{
if(b_alive == true)
	{
	SDL_Rect src;
	src.x = i_curFrame * width();
	src.y = i_curAction * height();
	src.w = width();
	src.h = height();

	SDL_Rect dst;
	dst.x = (int)f_pos[X];
	dst.y = (int)f_pos[Y];
	dst.w = width();
	dst.h = height();

	SDL_BlitSurface(s_sprite, &src, screen, &dst);
	}
}
