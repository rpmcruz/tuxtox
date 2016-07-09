/***************************************************************************
                          main.cpp  -  main file
                             -------------------
    begin                : Sáb Fev  8 19:12:09 WET 2003
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "includes.h"
#include "gamemanager.h"

void changeToCurDir(char *invocation);

int main(int argc, char *argv[])
{
cout << "BobbleBubble version 0.2, Copyright (C) 2003-04 Ricardo Cruz\n\n"
				"BobbleBubble comes with ABSOLUTELY NO WARRANTY.\n"
				"This is free software, and you are welcome to redistribute it\n"
				"under certain conditions. See the GNU General Public License for details.\n\n"
				"You should have received a copy of the GNU General Public License\n"
				"along with this program; if not, write to the Free Software\n"
				"Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n\n";

changeToCurDir(argv[0]);

char c_help[] = "BobbleBubble V0.2, written by Ricardo Cruz <rick2@aeiou.pt>\n"
		"\nUsage: bobblebubble [OPTIONS]\n"
		"\t--fullscreen\tFull screen video mode\n"
//		"\t--nosound\tRuns without sound (not implemented yet)\n"
		"\t--version\tShows program's version\n"
		"\t--help\tShows this help :-)\n\n";
char c_version[]= "BobbleBubble V0.2, written by Ricardo Cruz <rick2@aeiou.pt>\n\n";
bool b_fullScreen = false;

for (int i_argumentsLoop = 1; i_argumentsLoop < argc; i_argumentsLoop++)
	{
	if (strstr(argv[i_argumentsLoop], "--version") || strstr(argv[i_argumentsLoop], "-v"))
		{
		cout << c_version;
		return 0;
		}
	else if (strstr(argv[i_argumentsLoop], "--help") || strstr(argv[i_argumentsLoop], "-h"))
		{
		cout << c_help;
		return 0;
		}
	else if (strstr(argv[i_argumentsLoop], "--fullscreen") || strstr(argv[i_argumentsLoop], "-f"))
		b_fullScreen = true;
	}

// initialize the SDL library
if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	cerr << "Couldn't initialize SDL: " << SDL_GetError() << endl;
	return 1;
	}
// clean up on exit
atexit(SDL_Quit);

// initialize the display
SDL_Surface *s_screen;
if(b_fullScreen == true)
	s_screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 16, SDL_FULLSCREEN);
else
	s_screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 16, SDL_SWSURFACE);
if (s_screen == NULL)
	{
	cerr << "Couldn't set " << SCREENWIDTH << "x" << SCREENHEIGHT << "x16 video mode: " << SDL_GetError() << endl;
	return 1;
	}

SDL_WM_SetCaption("BobbleBubble", "BobbleBubble game");
SDL_Surface *icon = IMG_Load("icon.xpm");
SDL_WM_SetIcon(icon, NULL);
SDL_FreeSurface(icon);

// run game
runGame(s_screen);

cout << "Thank you for playing BobbleBubble!\n";
cout << "This game has kept you addictive for " << SDL_GetTicks() / 60000 << " minutes.\n\n";

SDL_FreeSurface(s_screen);
return 0;
}

void changeToCurDir(char *invocation)
{
int dirpath;
for(dirpath = strlen(invocation); invocation[dirpath] != '/' && dirpath != 0; dirpath--);

if(dirpath != 0)
	{
	char path[strlen(invocation)];
	int c;
	for(c = 0; c < dirpath; c++)
		path[c] = invocation[c];
	path[c] = '\0';

	int ret = chdir(path);
	if(ret != 0)
		cerr << "Error: current working directory could not be changed.\n"
						"Directory in cause: " << path <<
						"chdir() return number: " << ret << endl;
	}
}
