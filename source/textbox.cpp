#include "sdlFiles.h"
#include "defs.h"
extern SDL_Surface *screen;
extern SDL_Surface *text;
SDL_Surface *textBox[9] = { NULL };
extern int xInitial;
extern int yInitial;
extern int tilesX;
extern int tilesY;
void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination);
int getCenter(int a, int b);
//Render the textbox
//Todo: Implement dynamic resizing based on file length as well as space locations
void doTextBox(int posY)
{
	int y = 0;
	int maxx = 5;
	int maxy = 3;
	int xcounter = 0;
	int ycounter = 0;
	int x = getCenter(tilesX*2+1,maxx)*TILE_SIZE+xInitial;
	if (posY < 3)
		y = yInitial;
	else
		y = yInitial+TILE_SIZE*(tilesY+1);
	apply_surface(x+TILE_SIZE,y,textBox[0],screen);
	for (ycounter = 0; ycounter < maxy; ycounter++)
	{
		for (xcounter = 0; xcounter < maxx; xcounter++)
		{
			if (xcounter == 0)
			{
				if (ycounter == 0)
					apply_surface(x,y,textBox[4],screen);
				else if (ycounter == maxy-1)
					apply_surface(x,y+(ycounter*TILE_SIZE),textBox[7],screen);
				else
					apply_surface(x,y+(ycounter*TILE_SIZE),textBox[8],screen);
			}
			else if (xcounter == maxx-1)
			{
				if (ycounter == 0)
					apply_surface(x+(xcounter*TILE_SIZE),y,textBox[3],screen);
				else if (ycounter == maxy-1)
					apply_surface(x+(xcounter*TILE_SIZE),y+(ycounter*TILE_SIZE),textBox[6],screen);
				else
					apply_surface(x+(xcounter*TILE_SIZE),y+(ycounter*TILE_SIZE),textBox[1],screen);
			}
			else
			{
				if (ycounter == 0)
					apply_surface(x+(xcounter*TILE_SIZE),y+(ycounter*TILE_SIZE),textBox[2],screen);
				else if (ycounter == maxy-1)
					apply_surface(x+(xcounter*TILE_SIZE),y+(ycounter*TILE_SIZE),textBox[5],screen);
				else 
					apply_surface(x+(xcounter*TILE_SIZE),y+(ycounter*TILE_SIZE),textBox[0],screen);
			}
		}
	}
	apply_surface(x+TILE_SIZE, y+TILE_SIZE, text, screen);
}