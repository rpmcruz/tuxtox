/***************************************************************************
                          enemie.h  -  description
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

#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

/**Responsible for enemy moviment and turning into bobble.
  *@author Ricardo Cruz
  */

class Enemy : public Sprite
{
public:
	Enemy(char *filename, int x, int y, int curAction, int totActions, int totFrames);
	~Enemy();

	void update(float f_diffTicks);

private:
	int i_horDirection;
};

#endif
