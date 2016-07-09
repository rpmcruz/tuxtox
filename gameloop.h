/***************************************************************************
                          gameloop.h  -  description
                             -------------------
    begin                : Seg Mar 3 2003
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

#ifndef GAMELOOP_H
#define GAMELOOP_H

/**
  *@author Ricardo Cruz
  */

class Player;
class Bobble;
class Enemy;
class Level;

class GameLoop
{
public: 
	GameLoop();
	~GameLoop();

	int runLevel(SDL_Surface *screen, char *levelfile, int level, int pnb);

	int total_levels;

private:
	void keyEvents();
	void createBobble(int x, int y, int dir, int enemy);
	void checkSpritesForLevelCollisions(Sprite *sprite, int type);
	void blinkScreen(SDL_Surface *screen, int backcolor);

	bool testCollision(int firstX, int firstY, int firstWidth, int firstHeight, int secondX, int secondY, int secondWidth, int secondHeight);
  /* This functions splits the collider from the collided, and returns the side
     where the collision happen. Pass false, so that it only returns side of col. */
  int splitCollision(Sprite* collider, Sprite* collided, bool split);

	// variables:
	int gamedone;
	int totPlayers, totEnemies;
	Player *player[2];
	Bobble *bobble[MAXBOBBLES];
	Enemy *enemy[MAXENEMIES];
	Level *level;
};

#endif
