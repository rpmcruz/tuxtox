/***************************************************************************
                          player.cpp  -  description
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

#include "includes.h"
#include "player.h"

Player::Player(char *filename, int x, int y, int curAction, int totActions, int totFrames) : Sprite(filename, x, y, curAction, totActions, totFrames)
{
f_vel[X] = HORVEL;
f_vel[Y] = VERVEL;

i_horDirection = i_curAction;

b_floorColliding = false;

b_jumping = false;
b_alive = true;
i_shooting = 0;
}

Player::~Player()
{
}

void Player::jump()
{
b_jumping = false;
b_floorColliding = true;
b_direction[UP] = true;
}

bool Player::jumping()
{
return b_jumping;
}

int Player::horDirection()
{
return i_horDirection;
}

void Player::setShooting()
{
i_shooting = SDL_GetTicks() + SHOOT_IMG_TIME;
}

void Player::floorColliding(bool value)
{
b_floorColliding = value;
}

void Player::update(float f_diffTicks)
{
if(b_alive == true)
	{
	// check direction
	if(b_direction[LEFT] == true)
		i_horDirection = LEFT;
	else if(b_direction[RIGHT] == true)
		i_horDirection = RIGHT;
	i_curAction = i_horDirection;

	if(i_shooting > 0)
		if(i_shooting < SDL_GetTicks())
			i_shooting = 0;

	if(i_shooting > 0 && i_horDirection == LEFT)
		i_curAction = LSHOOT;
	else if(i_shooting > 0 && i_horDirection == RIGHT)
		i_curAction = RSHOOT;

	if(b_direction[UP] == true && b_jumping == false && b_floorColliding == true)
		{
		b_jumping = true;
		f_jumpTimer = SDL_GetTicks();
		}

	if(b_jumping == true)		// check if jump continues to happen
		if(SDL_GetTicks() - f_jumpTimer > JUMPSPEEDTIME*1000)
			if(SDL_GetTicks() - f_jumpTimer > (JUMPSPEEDTIME + JUMPAIRTIME)*1000)
				b_jumping = false;

	if(b_jumping == true)
		setDirection(UP, true);
	else
		setDirection(DOWN, true);

	updatePos(f_diffTicks);
	}
}
