/***************************************************************************
                          bobble.h  -  description
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

#ifndef BOBBLE_H
#define BOBBLE_H

#include "sprite.h"

/**Controls the bobbles created by players and the level. Can contain enimies and bonus.
  *@author Ricardo Cruz
  */

class Bobble : public Sprite
{
public: 
	Bobble(char *filename, int x, int y, int curAction, int totActions, int totFrames);
	~Bobble();

	void shoot(int dir);
	bool shooting();

	int enemy();
	void setEnemy(int enemy);
	void resetEnemy();

	void update(float f_diffTicks);

private:
	bool b_shooting;
	float f_shootTimer;

	int i_enemy;
};

#endif
