/***************************************************************************
                          level.h  -  description
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

#ifndef LEVEL_H
#define LEVEL_H


/**This class is responsible for the level look and objects positions.
  *@author Ricardo Cruz
  */

class Level
{
public: 
	Level(char *filename, int level, SDL_Surface *screen);
	~Level();

	int totEnemies();
	int enemiePos(int dir, int enemyNb);
	int playerPos(int dir, int playerNb);
	int totalLevels();

	bool groundCollision(int x, int y, int width, int height);
	int checkCollision(int dir, int oldPosX, int oldPosY, int posX, int posY, int width, int height);

	void drawArea(SDL_Surface *screen, int x, int y, int width, int height);
	void drawArea(SDL_Surface *screen, int x, int y, int width, int height, int backcolor);

private:
	int tileNumber(bool dir, bool pos, int x, int y, int width, int height);

	char* filemanager(const char *path, const char *group, const char *entry);


	SDL_Surface *s_tile;
	bool b_tiles[TOTCOLS][TOTROWS];

	int i_enemiePos[MAXENEMIES][2];
	int i_playerPos[2][2];
	int i_totEnemies;

	int i_shadowColor, i_backColor;
	int i_totalLevels;
};

#endif
