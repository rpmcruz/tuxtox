/***************************************************************************
                          bobble.cpp  -  description
                             -------------------
    begin                : Qui Fev 27 2003
    copyright            : (C) 2003 by Ricardo Cruz
    email                : <ricardo.pdm.cruz@gmail.com>
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
#include "bobble.h"

Bobble::Bobble(char *filename, int x, int y, int curAction, int totActions, int totFrames) : Sprite(filename, x, y, curAction, totActions, totFrames)
{
f_vel[X] = BOBHORVEL;
f_vel[Y] = BOBVERVEL;

f_shootTimer = 0;
b_shooting = false;

i_enemy = -1;

b_alive = false;
}

Bobble::~Bobble()
{
}

void Bobble::shoot(int dir)
{
f_shootTimer = SDL_GetTicks();

for(int i = 0; i < 4; i++)
	b_direction[i] = false;

if(dir != -1)
	{
	b_shooting = true;
	setDirection(dir, true);
	}
}

bool Bobble::shooting()
{
return b_shooting;
}

int Bobble::enemy()		// give enemy
{
return i_enemy;
}

void Bobble::setEnemy(int enemy)
{
i_enemy = enemy;
i_curAction = ACTENEMY1;
}

void Bobble::resetEnemy()		// destroy his info
{
i_enemy = -1;
i_curAction = ACTDEFAULT;
}

void Bobble::update(float f_diffTicks)
{
if(b_alive == true)
	{
	if(SDL_GetTicks() - f_shootTimer > BOBDESTROYTIME*1000)
		{
		b_alive = false;
		return;
		}

//	if((b_direction[LEFT] == true || b_direction[RIGHT] == true) && b_shooting == false)
//		{
//		b_shooting = true;
//		f_shootTimer = SDL_GetTicks();
//		}

	if(b_shooting == true)		// check if shooting continues to happen
		if(SDL_GetTicks() - f_shootTimer > BOBSHOOTTIME*1000)
			{
			b_shooting = false;
			setDirection(LEFT, false);
			setDirection(RIGHT, false);
			}

	if(b_shooting == false)
		setDirection(UP, true);

	updatePos(f_diffTicks);
	}
}
