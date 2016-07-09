/***************************************************************************
                          gameloop.cpp  -  description
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

#include <math.h>
#include "includes.h"
#include "level.h"
#include "player.h"
#include "bobble.h"
#include "enemy.h"
#include "gameloop.h"

GameLoop::GameLoop()
  : total_levels(0)
{
}

GameLoop::~GameLoop()
{
}

int GameLoop::runLevel(SDL_Surface *screen, char *levelfile, int levelnb, int pnb)
{
totPlayers = pnb;

level = new Level("data/levels", levelnb, screen);
total_levels = level->totalLevels();

player[0] = new Player("data/smalltux1.bmp", level->playerPos(X,0), level->playerPos(Y,0), RIGHT, 4, 3);
if(totPlayers == TWOPLAYER)
	player[1] = new Player("data/smalltux2.bmp", level->playerPos(X,1), level->playerPos(Y,1), LEFT, 4, 3);

totEnemies = level->totEnemies();

// grab a random image of the enemy
srand(SDL_GetTicks());
int enemynb = (rand()%3)+1;   //
char str[26];
sprintf(str, "data/enemy%d.bmp", enemynb);

for(int i = 0; i < level->totEnemies(); i++)
	enemy[i] = new Enemy(str, level->enemiePos(X, i), level->enemiePos(Y, i), LEFT, 2, 3);

for(int i = 0; i < MAXBOBBLES; i++)		// update pos, based on the keys
	bobble[i] = new Bobble("data/bubble.bmp", 0, 0, 0, 2, 1);

level->drawArea(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);

float f_diffTicks;
int i_lastTick = SDL_GetTicks();

gamedone = 0;
while(gamedone == 0)
	{
	/* key events */
	keyEvents();		// send keys to players

	f_diffTicks = SDL_GetTicks() - i_lastTick;
	f_diffTicks /= NORMALDELAY;
	i_lastTick = SDL_GetTicks();

	/* updating moviments */
	for(int i = 0; i < totPlayers; i++)		// update pos, based on the keys
		player[i]->update(f_diffTicks);
 for(int i = 0; i < totEnemies; i++)		// update pos, based on the keys
		enemy[i]->update(f_diffTicks);
	for(int i = 0; i < MAXBOBBLES; i++)		// update pos, based on the keys
		bobble[i]->update(f_diffTicks);

	for(int bob = 0; bob < MAXBOBBLES; bob++)		// check if a bobble has an enemy
		{
		if(bobble[bob]->alive() == false && bobble[bob]->enemy() != -1)
			{
			int ene = bobble[bob]->enemy();
			bobble[bob]->resetEnemy();
			enemy[ene]->setPos(bobble[bob]->pos(X), X);
			enemy[ene]->setPos(bobble[bob]->pos(Y), Y);
			enemy[ene]->setAlive(true);
			}
		}

	/* checking collisions */
	for(int i = 0; i < totPlayers; i++)
		checkSpritesForLevelCollisions(player[i], PLAYER);
 for(int i = 0; i < totEnemies; i++)
		checkSpritesForLevelCollisions(enemy[i], ENEMY);
	for(int i = 0; i < MAXBOBBLES; i++)
		checkSpritesForLevelCollisions(bobble[i], BOBBLE);

	for(int i = 0; i < totPlayers; i++)	// this is only used for jump
		player[i]->floorColliding(level->groundCollision(player[i]->pos(X), player[i]->pos(Y)+1, player[i]->width(), player[i]->height()));

	for(int bob = 0; bob < MAXBOBBLES; bob++)		// test collision between enimie and bobbles
		for(int ene = 0; ene < totEnemies; ene++)		// test collision between enimie and bobbles
			if(bobble[bob]->alive() && enemy[ene]->alive() && bobble[bob]->enemy() == -1 && bobble[bob]->shooting())
				if(testCollision(bobble[bob]->pos(X), bobble[bob]->pos(Y), bobble[bob]->width(), bobble[bob]->height(), enemy[ene]->pos(X), enemy[ene]->pos(Y), enemy[ene]->width(), enemy[ene]->height()) == true)
					{
					createBobble(enemy[ene]->pos(X), enemy[ene]->pos(Y), -1, ene);
					enemy[ene]->setAlive(false);
					bobble[bob]->setAlive(false);
					}

	for(int pla = 0; pla < totPlayers; pla++)		// test collision between players and enimie
		for(int ene = 0; ene < totEnemies; ene++)		// test collision between players and enimie
			if(player[pla]->alive() == true && enemy[ene]->alive() == true)
				if(testCollision(player[pla]->pos(X), player[pla]->pos(Y), player[pla]->width(), player[pla]->height(), enemy[ene]->pos(X), enemy[ene]->pos(Y), enemy[ene]->width(), enemy[ene]->height()) == true)
					player[pla]->setAlive(false);
	for(int bob = 0; bob < MAXBOBBLES; bob++)		// test collision between players and bobbles
		for(int pla = 0; pla < totPlayers; pla++)
			if(player[pla]->alive() == true && bobble[bob]->alive() == true && bobble[bob]->shooting() == false)
				if(testCollision(player[pla]->pos(X), player[pla]->pos(Y), player[pla]->width(), player[pla]->height(), bobble[bob]->pos(X), bobble[bob]->pos(Y), bobble[bob]->width(), bobble[bob]->height()))
					{
          // destroy the bubble if we have "entered" in it too much.
          if(testCollision(player[pla]->pos(X), player[pla]->pos(Y),
                 player[pla]->width(), player[pla]->height(), 
                 bobble[bob]->pos(X)+bobble[bob]->width()/3,
                 bobble[bob]->pos(Y)+bobble[bob]->height()/3,
                 (int)(bobble[bob]->width()-(2*(bobble[bob]->width()/3))),
                 (int)(bobble[bob]->height()-(2*(bobble[bob]->height()/3)))))
            {  // kill bubble
            bobble[bob]->setAlive(false);
            bobble[bob]->resetEnemy();
            }
          else
            {
            int dir = splitCollision(player[pla], bobble[bob], false);
            if(dir == DOWN && SDL_GetKeyState(NULL)[SDLK_UP])  // hack
              player[pla]->jump();  // jupm over bubble

            else if(dir == LEFT)  // push bubble
              bobble[bob]->setPos((int)(bobble[bob]->pos(X)+(2.5*f_diffTicks)), X);
            else if(dir == RIGHT)  // push bubble
              bobble[bob]->setPos((int)(bobble[bob]->pos(X)-(2.5*f_diffTicks)), X);
					  }
          }

	/* drawing level */
	for(int i = 0; i < totPlayers; i++)		// refresh last player's pos
		level->drawArea(screen, player[i]->oldPos(X), player[i]->oldPos(Y), player[i]->width(), player[i]->height());
	for(int i = 0; i < totEnemies; i++)		// refresh last player's pos
		level->drawArea(screen, enemy[i]->oldPos(X), enemy[i]->oldPos(Y), enemy[i]->width(), enemy[i]->height());
	for(int i = 0; i < MAXBOBBLES; i++)		// refresh last player's pos
		level->drawArea(screen, bobble[i]->oldPos(X), bobble[i]->oldPos(Y), bobble[i]->width(), bobble[i]->height());
	/* drawing objects */
	for(int i = 0; i < totPlayers; i++)		// put player in screen
		player[i]->draw(screen, 0);
	for(int i = 0; i < totEnemies; i++)		// put player in screen
		enemy[i]->draw(screen, 0);
	for(int i = 0; i < MAXBOBBLES; i++)		// put player in screen
		bobble[i]->draw(screen, 0);
	/* updating screen */
	SDL_UpdateRect(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);

	/* checking if it's time to exit level */
	// see if players are all dead, and if affirmitive, goes back to the menu
	bool alive = false;
	for(int i = 0; i < totPlayers; i++)
		if(player[i]->alive() == true)
			{
			alive = true;
			break;
			}
	if(alive == false)
		{
		blinkScreen(screen, SDL_MapRGB(screen->format, 255, 0, 0));
		gamedone = PREVLEVEL;
		}
	// see if enemies are all dead, and if affirmitive, continue to the next level
	alive = false;
	for(int i = 0; i < totEnemies; i++)
		if(enemy[i]->alive() == true)
			{
			alive = true;
			break;
			}
	for(int i = 0; i < MAXBOBBLES; i++)
		if(bobble[i]->enemy() != -1)
			{
			alive = true;
			break;
			}
	if(alive == false)
		{
		blinkScreen(screen, SDL_MapRGB(screen->format, 0, 255, 0));
		gamedone = NEXTLEVEL;
		}
	}

// now do the cleanups
delete level;
for(int i = 0; i < totPlayers; i++)
	delete player[i];
for(int i = 0; i < totEnemies; i++)
	delete enemy[i];
for(int i = 0; i < MAXBOBBLES; i++)
	delete bobble[i];

return gamedone;
}

void GameLoop::blinkScreen(SDL_Surface *screen, int backcolor)
{
for(int i = 0; i < NB_OF_BLINKS; i++)
	{
	level->drawArea(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT, backcolor);
	for(int i = 0; i < totPlayers; i++)
		player[i]->draw(screen, 0);
	for(int i = 0; i < totEnemies; i++)
		enemy[i]->draw(screen, 0);
	for(int i = 0; i < MAXBOBBLES; i++)
		bobble[i]->draw(screen, 0);
	SDL_UpdateRect(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);

	SDL_Delay(BLINK_DELAY);

	level->drawArea(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);
	for(int i = 0; i < totPlayers; i++)
		player[i]->draw(screen, 0);
	for(int i = 0; i < totEnemies; i++)
		enemy[i]->draw(screen, 0);
	for(int i = 0; i < MAXBOBBLES; i++)
		bobble[i]->draw(screen, 0);
	SDL_UpdateRect(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);

	SDL_Delay(BLINK_DELAY);
	}
}

void GameLoop::checkSpritesForLevelCollisions(Sprite *sprite, int type)
{
for(int dir = 0; dir < 4; dir++)		// check for collisions
	{
	if(sprite->alive() == true)
		{
		if(sprite->direction(dir) == false)
			continue;
		if((type == PLAYER || type == ENEMY) && dir == UP)
			continue;

		int entrance = level->checkCollision(dir, sprite->oldPos(X), sprite->oldPos(Y), sprite->pos(X), sprite->pos(Y), sprite->width(), sprite->height());

		if(entrance != 0)
			{
			sprite->setColliding(dir, true);
			if(dir == LEFT || dir == RIGHT)
				sprite->setPos(entrance, X);
			if(dir == DOWN || dir == UP)
				sprite->setPos(entrance, Y);
			}
		}
	}
}

void GameLoop::createBobble(int x, int y, int dir, int enemy)		// give dir = -1 if you don't want to shoot the bobble
{
for(int i = 0; i < MAXBOBBLES; i++)		// update pos, based on the keys
	if(bobble[i]->alive() == false)
		{
		bobble[i]->setPos(x, X);
		bobble[i]->setPos(y, Y);
		bobble[i]->shoot(dir);
		if(enemy != -1)
			bobble[i]->setEnemy(enemy);
		bobble[i]->setAlive(true);
		break;
		}
}

void GameLoop::keyEvents()
{
SDL_Event event;

/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
/* more events on the event queue, our while loop will exit when */
/* that occurs.                                                  */
while(SDL_PollEvent(&event))
	{
	// testing SDL_KEYDOWN, SDL_KEYUP and SDL_QUIT events
	switch(event.type)
		{
		case SDL_KEYDOWN:	// key pressed
			switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
					player[0]->setDirection(LEFT, true);
					break;
				case SDLK_RIGHT:
					player[0]->setDirection(RIGHT, true);
					break;
				case SDLK_UP:
					player[0]->setDirection(UP, true);
					break;
				case SDLK_RETURN:
					if(player[0]->alive() == true)
						createBobble(player[0]->pos(X), player[0]->pos(Y), player[0]->horDirection(), -1);
					player[0]->setShooting();
					break;
				case SDLK_a:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(LEFT, true);
					break;
				case SDLK_d:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(RIGHT, true);
					break;
				case SDLK_w:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(UP, true);
					break;
				case SDLK_LSHIFT:
					if(totPlayers == TWOPLAYER)
						{
						if(player[1]->alive() == true)
							createBobble(player[1]->pos(X), player[1]->pos(Y), player[1]->horDirection(), -1);
						player[1]->setShooting();
						}
					break;
				default:
					break;
				}
			break;
		case SDL_KEYUP:	// key released
			switch(event.key.keysym.sym)
				{
				case SDLK_LEFT:
					player[0]->setDirection(LEFT, false);
					break;
				case SDLK_RIGHT:
					player[0]->setDirection(RIGHT, false);
					break;
				case SDLK_UP:
					player[0]->setDirection(UP, false);
					break;
				case SDLK_a:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(LEFT, false);
					break;
				case SDLK_d:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(RIGHT, false);
					break;
				case SDLK_w:
					if(totPlayers == TWOPLAYER)
						player[1]->setDirection(UP, false);
					break;
				case SDLK_ESCAPE:
					gamedone = RETURN;
					break;
				case SDLK_PLUS:  // cheat to go to next level (key +)
					gamedone = NEXTLEVEL;
					break;
				default:
					break;
				}
			break;
		case SDL_QUIT:	// window closed
			gamedone = RETURN;
			break;
		default:
			break;
		}
	}
}

bool GameLoop::testCollision(int firstX, int firstY, int firstW, int firstH, int secondX, int secondY, int secondW, int secondH)
{
// very simple box collision detection
return !(firstX + firstW < secondX || secondX + secondW < firstX ||
         firstY + firstH < secondY || secondY + secondH < firstY);
}

int GameLoop::splitCollision(Sprite* collider, Sprite* collided, bool split)
{  // collider should be the collider
// split collisions and return the side of the collision
float dir[4];
dir[LEFT]= collider->pos(X) + collider->width() - collided->pos(X) + 1;
dir[RIGHT] = collider->pos(X) - (collided->pos(X) + collided->width()) - 1;
dir[DOWN] = collider->pos(Y) + collider->height() - collided->pos(Y) + 1;
dir[UP]   = collider->pos(Y) - (collided->pos(Y) + collided->height()) - 1;

// check the inferior value and do the consquencis
// collider line splits up cars, following lines do the impact.
// TODO: maybe it would be a good idea to enable collision in both
// X and Y axis (ie. on diagonal movements).
int inf_dir = 0;
for(int i = 1; i < 4; i++)
  if(fabs(dir[i]) < fabs(dir[inf_dir]))
    inf_dir = i;

if(split)
  {
  if(inf_dir == LEFT || inf_dir == RIGHT)
    collider->setPos((int)(collider->pos(X)-dir[inf_dir]), X);
  else// if(inf_dir == LEFT || inf_dir == RIGHT)
    collider->setPos((int)(collider->pos(Y)-dir[inf_dir]), Y);
  }

return inf_dir;            
}
