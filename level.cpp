/***************************************************************************
                          level.cpp  -  description
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
#include "level.h"

Level::Level(char *filename, int level, SDL_Surface *screen)
{
char levelnb[26];
sprintf(levelnb, "[LEVEL%d]", level);

s_tile = SDL_LoadBMP(filemanager(filename, levelnb, "tile="));
if (s_tile == NULL)
	cerr << "Couldn't open tile file: " << filename << endl;

i_totalLevels = atoi(filemanager(filename, "[LEVELS]", "totallevels="));

int redShadow = atoi(filemanager(filename, levelnb, "redshadow="));
int greenShadow = atoi(filemanager(filename, levelnb, "greenshadow="));
int blueShadow = atoi(filemanager(filename, levelnb, "blueshadow="));
i_shadowColor = SDL_MapRGB(screen->format, redShadow, greenShadow, blueShadow);

int redBackClr = atoi(filemanager(filename, levelnb, "redbackclr="));
int greenBackClr = atoi(filemanager(filename, levelnb, "greenbackclr="));
int blueBackClr = atoi(filemanager(filename, levelnb, "bluebackclr="));
i_backColor = SDL_MapRGB(screen->format, redBackClr, greenBackClr, blueBackClr);

i_playerPos[0][X] = atoi(filemanager(filename, levelnb, "pla1-col=")) * s_tile->w;
i_playerPos[0][Y] = atoi(filemanager(filename, levelnb, "pla1-row=")) * s_tile->h;
i_playerPos[1][X] = atoi(filemanager(filename, levelnb, "pla2-col=")) * s_tile->w;
i_playerPos[1][Y] = atoi(filemanager(filename, levelnb, "pla2-row=")) * s_tile->h;

i_totEnemies = 0;

char buffer[TOTROWS+20];
// example: row15=1111110000000000000011;
for(int row = 0; row < TOTROWS; row++)
	{
	sprintf(buffer, "row%2d=", row);
	char *entry = filemanager(filename, levelnb, buffer);

	for(int col = 0; col < TOTCOLS; col++)
		{
		if(entry[col] == '0')
			b_tiles[col][row] = false;
		else if(entry[col] == '1')
			b_tiles[col][row] = true;
		else if(entry[col] == '2')
			{
			b_tiles[col][row] = false;

			i_enemiePos[i_totEnemies][X] = col * s_tile->w;
			i_enemiePos[i_totEnemies][Y] = row * s_tile->h;

			i_totEnemies++;
			}
		else
			cerr << "ERROR: character different from 0, 1 or 2 found in the level file\n";
		}
	}
}

Level::~Level()
{
SDL_FreeSurface(s_tile);
}

int Level::totEnemies()
{
return i_totEnemies;
}

int Level::enemiePos(int dir, int enemyNb)
{
return i_enemiePos[enemyNb][dir];
}

int Level::playerPos(int dir, int playerNb)
{
return i_playerPos[playerNb][dir];
}

bool Level::groundCollision(int x, int y, int width, int height)
{
int tileNbX[2];
int tileNbY;
tileNbX[FIRST] = tileNumber(COL, FIRST, x, y, width, height);
tileNbX[LAST] = tileNumber(COL, LAST, x, y, width, height)+1;
tileNbY = tileNumber(ROW, LAST, x, y, width, height);

for(; tileNbX[FIRST] < tileNbX[LAST]; tileNbX[FIRST]++)
	if(b_tiles[tileNbX[FIRST]][tileNbY] == true)
		return true;

return false;
}

int Level::tileNumber(bool dir, bool pos, int x, int y, int width, int height)
{
int number;

if(dir == COL)
	{
	x += HORENTERPIX;		// this is to allow players to enter a bit on the tile corners
	width -= HORENTERPIX*2;		// and to give a cool effect
	if(pos == FIRST)
		number = x/s_tile->w;
	if(pos == LAST)
		number = (x+width)/s_tile->w;
	}
else if(dir == ROW)
	{
	if(pos == FIRST)
		number = y/s_tile->h;
	if(pos == LAST)
		number = (y+height-1)/s_tile->h;		// -1 is done because of the collision code
	}

return number;
}

int Level::checkCollision(int dir, int oldPosX, int oldPosY, int posX, int posY, int width, int height)
{
if(dir == DOWN && posY > oldPosY)
	{
	int oldLastRow = tileNumber(ROW, LAST, oldPosX, oldPosY, width, height);
	int curLastRow = tileNumber(ROW, LAST, posX, posY, width, height);

	int firstCol = tileNumber(COL, FIRST, oldPosX, oldPosY, width, height);
	int lastCol = tileNumber(COL, LAST, oldPosX, oldPosY, width, height);

	for(; oldLastRow != curLastRow; oldLastRow++)
		for(; firstCol <= lastCol; firstCol++)
			if(b_tiles[firstCol][oldLastRow+1] == true)
				return ((oldLastRow+1) * s_tile->h) - height;		// return the row he should go to
	}
if(dir == UP && posY < oldPosY)
	{
	int oldFirstRow = tileNumber(ROW, FIRST, oldPosX, oldPosY, width, height);
	int curFirstRow = tileNumber(ROW, FIRST, posX, posY, width, height);

	int firstCol = tileNumber(COL, FIRST, oldPosX, oldPosY, width, height);
	int lastCol = tileNumber(COL, LAST, oldPosX, oldPosY, width, height);

	for(; oldFirstRow != curFirstRow; oldFirstRow--)
		for(; firstCol <= lastCol; firstCol++)
			if(b_tiles[firstCol][oldFirstRow-1] == true)
				return (oldFirstRow) * s_tile->h;		// return the row he should go to
	}
if(dir == LEFT && posX < oldPosX)
	{
	int oldFirstCol = tileNumber(COL, FIRST, oldPosX, oldPosY, width, height);
	int curFirstCol = tileNumber(COL, FIRST, posX, posY, width, height);

	int firstRow = tileNumber(ROW, FIRST, oldPosX, oldPosY, width, height);
	int lastRow = tileNumber(ROW, LAST, oldPosX, oldPosY, width, height);

	for(; oldFirstCol != curFirstCol; oldFirstCol--)
		for(; firstRow <= lastRow; firstRow++)
			if(b_tiles[oldFirstCol-1][firstRow] == true)
				return ((oldFirstCol) * s_tile->w)-HORENTERPIX;		// return the row he should go to
	}
if(dir == RIGHT && posX > oldPosX)
	{
	int oldLastCol = tileNumber(COL, LAST, oldPosX, oldPosY, width, height);
	int curLastCol = tileNumber(COL, LAST, posX, posY, width, height);

	int firstRow = tileNumber(ROW, FIRST, oldPosX, oldPosY, width, height);
	int lastRow = tileNumber(ROW, LAST, oldPosX, oldPosY, width, height);

	for(; oldLastCol != curLastCol; oldLastCol++)
		for(; firstRow <= lastRow; firstRow++)
			if(b_tiles[oldLastCol+1][firstRow] == true)
				return (((oldLastCol+1) * s_tile->w) - width-1+HORENTERPIX);		// return the row he should go to
	}

return 0;
}

void Level::drawArea(SDL_Surface *screen, int xpos, int ypos, int width, int height)
{
SDL_Rect r_src;
r_src.x = r_src.y = 0;
r_src.w = s_tile->w;
r_src.h = s_tile->h;

SDL_Rect r_dst;
r_dst.x = r_dst.y = 0;
r_dst.w = r_src.w;
r_dst.h = r_src.h;

int col = xpos/s_tile->w;
int row = ypos/s_tile->h;
int lastcol = ((xpos+width)/s_tile->w)+1;
int lastrow = ((ypos+height)/s_tile->h)+1;

//b_tiles[TOTCOLTILES][TOTROWTILES]
for(col = 0; col < lastcol; col++)
	for(row = 0; row < lastrow; row++)
		{
		r_dst.x = col * s_tile->w;
		r_dst.y = row * s_tile->h;

		if(b_tiles[col][row] == true)
			SDL_BlitSurface(s_tile, &r_src, screen, &r_dst);
		else
			{
			SDL_FillRect(screen, &r_dst, i_backColor);

			if(row-1 >= 0)
				if(b_tiles[col][row-1] == true)
					{
					r_dst.h /= 2;
					SDL_FillRect(screen, &r_dst, i_shadowColor);
					r_dst.h = r_src.h;
					}
			if(col-1 >= 0)
				if(b_tiles[col-1][row] == true)
					{
					r_dst.w /= 2;
					SDL_FillRect(screen, &r_dst, i_shadowColor);
					r_dst.w = r_src.w;
					}
			if(row-1 >= 0 && col-1 >= 0)
				if(b_tiles[col-1][row-1] == true)
					{
					r_dst.w /= 2;
					r_dst.h /= 2;
					SDL_FillRect(screen, &r_dst, i_shadowColor);
					r_dst.w = r_src.w;
					r_dst.h = r_src.h;
					}
			}
		}
}

void Level::drawArea(SDL_Surface *screen, int xpos, int ypos, int width, int height, int backcolor)
{   // hack to change the background color
int temp = i_backColor;
i_backColor = backcolor;
drawArea(screen, xpos, ypos, width, height);
i_backColor = temp;
}

int Level::totalLevels()
{
return i_totalLevels;
}

char* Level::filemanager(const char *path, const char *group, const char *entry)
{
FILE *file = fopen(path, "r");
if (file == NULL)
	{
	cerr << "File: " << path << " couldn't be open\n";
	return NULL;
	}

char buffer[1024];
static char entryvalue[1024];
char *ptr;
char *entryptr;
bool groupFound = false;
bool entryFound = false;

while ((ptr = fgets(buffer, sizeof(buffer), file)) != NULL)
	{
	if (ptr[0] == '#')	// comment
		continue;

	if((groupFound == true) && ((entryptr = strstr(ptr, entry)) != NULL))
		{
		entryptr += strlen(entry);

		char *semicolonptr;		// will point to ;
		strcpy(entryvalue, entryptr);

		if ((semicolonptr = strchr(entryvalue, ';')) == NULL)
			cerr << "ERROR: ; is missing in line: " << ptr << endl;
		else
			*semicolonptr = '\0';
		entryFound = true;

		break;
		}
	if (ptr[0] == '[')	// group symbol: [
		{
		groupFound = false;
		if (strstr(ptr, group) != NULL)
			groupFound = true;
		}
	}
fclose(file);
if (entryFound == false)
	{
	cerr << "ERROR: the " << entry << " entry was not found on " << group << " group in the file " << path << " ." << endl;
	return NULL;
	}
return entryvalue;

// example:
//filemanager("/home/rick2/.kde/share/config/kppprc", "[Account1]", "Password=");
}
