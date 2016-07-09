/***************************************************************************
                          enemie.cpp  -  description
                             -------------------
    begin                : Sáb Mar 1 2003
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
#include "enemy.h"

Enemy::Enemy(char *filename, int x, int y, int curAction, int totActions, int totFrames) : Sprite(filename, x, y, curAction, totActions, totFrames)
{
f_vel[X] = ENEHORVEL;
f_vel[Y] = ENEVERVEL;

setDirection(DOWN, true);
i_horDirection = curAction;

b_alive = true;
}

Enemy::~Enemy()
{
}

void Enemy::update(float f_diffTicks)
{
if(b_alive == true)
	{
	// check direction
	if(b_direction[LEFT] == true)
		i_horDirection = LEFT;
	else if(b_direction[RIGHT] == true)
		i_horDirection = RIGHT;
	i_curAction = i_horDirection;

	if(b_colliding[LEFT] == true)
		setDirection(RIGHT, true);
	if(b_colliding[RIGHT] == true)
		setDirection(LEFT, true);

	if(b_direction[LEFT] == false && b_direction[RIGHT] == false)
		if(b_colliding[DOWN] == true)
			setDirection(LEFT, true);

	updatePos(f_diffTicks);
	}
}
