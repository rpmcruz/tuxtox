/***************************************************************************
                          sprite.h  -  description
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

#ifndef SPRITE_H
#define SPRITE_H


/**Base class for player and bobble, contains vel, draw... and other basic code.
  *@author Ricardo Cruz
  */

class Sprite
{
public: 
	Sprite(char *filename, int x, int y, int curAction, int totActions, int totFrames);
	~Sprite();

	SDL_Surface* surface();
	int oldPos(int dir);
	int pos(int dir);
	void setPos(int pos, int dir);
	int width();
	int height();

	void setColliding(int dir, bool colliding);
	bool colliding(int dir);
	void setDirection(int direction, bool moving);
	bool direction(int dir);

	void setAlive(bool alive);
	bool alive();

	void setAction(int action);

	void updatePos(float f_diffTicks);
	void draw(SDL_Surface *screen, Uint32 color);

protected:
	SDL_Surface *s_sprite;

	float f_oldPos[2], f_pos[2], f_vel[2];
	bool b_direction[4];

	int i_curFrame, i_totFrames;
	int i_curAction, i_totActions;
	float f_chgFrameTimer;

	bool b_colliding[4];
	bool b_alive;
};

#endif
