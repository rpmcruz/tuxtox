/***************************************************************************
                          player.h  -  description
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

#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

/**This class is responsible for the players (the dragons).
  *@author Ricardo Cruz
  */

class Player : public Sprite
{
public:
	Player(char *filename, int x, int y, int curAction, int totActions, int totFrames);
	~Player();

  void jump();
	bool jumping();
	int horDirection();
	void setShooting();

	void floorColliding(bool value);

	void update(float f_diffTicks);

private:
	float f_jumpTimer;
	bool b_jumping;
	unsigned int i_shooting;

	bool b_floorColliding;	// this is a workaround

	int i_horDirection;
};

#endif
