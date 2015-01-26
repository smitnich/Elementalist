#include "gfx.h"
#include "level.h"
#include "objectDef.h"
extern Object *player;
SDL_Rect borderRect[4];
Level *getCurrentLevel();
//Draws the static map features, the objects, the player, and the textbox on the screen
void drawScreen()
{
	int x, y, xOffset = 0, yOffset = 0;
	Level *currentLevel = getCurrentLevel();
	bool doDir[4] = { 0, 0, 0, 0 };
	Terrain *terrain = NULL;
	Object *obj = NULL;
	//Do one tile more than we need so that we get partially drawn tiles correctly
	for (x = -NUM_TILES-1; x <= NUM_TILES+1; x++)
	{
		for (y = -NUM_TILES-1; y <= NUM_TILES+1; y++)
		{
			terrain = currentLevel->getTerrain(player->x + x, player->y + y);
			calculateMoveFraction(player->objMoveDir, player->objMoveFraction, &xOffset, &yOffset, doDir);
			if (terrain != NULL)
				terrain->draw(screen, x + NUM_TILES, y + NUM_TILES, -xOffset, -yOffset);
		}
	}
	//Draw the mouse if it is within bounds and should be drawn
	//Now do this again with objects; because objects can be drawn across two tiles, we want to make sure that
	//we don't draw over any of them.
	for (x = -NUM_TILES - 1; x <= NUM_TILES + 1; x++)
	{
		for (y = -NUM_TILES - 1; y <= NUM_TILES + 1; y++)
		{
			obj = currentLevel->getObject(player->x + x, player->y + y);
			if (obj != NULL)
				doDraw(obj, xOffset, yOffset, doDir);
		}
	}
	if (pointerX > -1 && pointerY > -1 && showCursor == true)
		apply_surface(pointerX,pointerY,cursor,screen);
	drawBorders();
}
void drawWrappedSprite(int x, int y, SDL_Surface* source, SDL_Surface* destination, int xWrap, int yWrap)
{
	//Make sure we're not over the tileSize
	int i;
	xWrap %= tileSize;
	yWrap %= tileSize;
	SDL_Rect *offset[2];
	SDL_Rect *portion[2];
	portion[0] = rectMake(0,0,tileSize-yWrap,tileSize-xWrap);
	portion[1] = rectMake((tileSize-xWrap) % tileSize, (tileSize-yWrap) % tileSize, yWrap, xWrap);
	offset[0] = rectMake(xWrap+x,yWrap+y,tileSize-yWrap,tileSize-xWrap);
	offset[1] = rectMake(x,y,yWrap,xWrap);
	for (i = 0; i < 2; i++)
	{
		if (portion[i]->h == 0)
			portion[i]->h = tileSize;
		if (portion[i]->w == 0)
			portion[i]->h = tileSize;
		SDL_BlitSurface( source, portion[i], destination, offset[i] );
		free(offset[i]);
		free(portion[i]);
	}
}
SDL_Rect* rectMake(int x, int y, int h, int w)
{
	SDL_Rect *output = new SDL_Rect;
	output->x = x;
	output->y = y;
	output->h = h;
	output->w = w;
	return output;
}
//Blit an entire image to the screen
void apply_surface (int x,int y,SDL_Surface* source,SDL_Surface* destination){
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, NULL, destination, &offset );
}
//Initialize the graphics and fill the screen with the background
void gfxInit()
{
	if (xInitial == 0)
		xInitial = 24;
	yInitial = getCenter(videoSizeY,tileSize*(tilesY*2+1));
	imgInit();
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 91, 91, 255));
	SDL_Flip(screen);
	borderRect[0].x = 0; borderRect[0].y = 0; borderRect[0].w = xInitial, borderRect[0].h = videoSizeY;
	borderRect[1].x = xInitial; borderRect[1].y = 0; borderRect[1].w = videoSizeX-xInitial; borderRect[1].h = yInitial;
	borderRect[2].x = xMax; borderRect[2].y = 0; borderRect[2].w = videoSizeX-xMax, borderRect[2].h = videoSizeY;
	borderRect[3].x = xInitial; borderRect[3].y = yMax; borderRect[3].w = xMax-xInitial; borderRect[3].h = videoSizeY-yMax;
}
//Apply a surface at a given coordinate, up to a certain width and height
void apply_surface(int x, int y, int width, int height, SDL_Surface* source, SDL_Surface* destination)
{
	if (width == 0)
		width = tileSize;
	if (height == 0)
		height = tileSize;
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_Rect portion;
	portion.x = 0;
	portion.w = 0;
	portion.y = 0;
	portion.h = 0;
	if (width > 0)
	{
		portion.x = 0;
		portion.w = width;
	}
	else if (width < 0)
	{
		width*=-1;
		portion.x = tileSize-width;
		offset.x += tileSize-width;
		portion.w = width;
	}
	if (height > 0)
	{
		portion.y = 0;
		portion.h = height;
	}
	else if (height < 0)
	{
		height*=-1;
		portion.y = tileSize-height;
		offset.y += tileSize-height;
		portion.h = height;
	}
	SDL_BlitSurface( source, &portion, destination, &offset );
	drawBorders();
}
void drawBorders()
{
	for (int i = 0; i < 4; i++)
	{
		SDL_FillRect(screen, &borderRect[i], SDL_MapRGB(screen->format, 91, 91, 255));
	}
}
//Draw a wall based on the type value passed in
void doWall(int type, int x, int y, int moveFractionX, int moveFractionY)
{
	apply_surface(x*tileSize+xInitial-moveFractionX,y*tileSize+yInitial-moveFractionY,wall[type],screen);
}
//Draw a sprite at a given position
void drawSprite(int drawX,int drawY,SDL_Surface* toDraw)
{
	apply_surface(drawX,drawY,toDraw,screen);
}
//Change whether or not to draw the cursor
void toggleCursor(bool value)
{
	showCursor = value;
}
//Change the level text to the current level name
void changeText()
{
	string textName;
	if (startLevelName.compare("") == 0)
		textName.assign(LevelStrings[currentLevelNum].data());
	else
		textName.assign(startLevelName);
	SDL_FreeSurface(text);
	text = TTF_RenderText_Solid( font, textName.c_str(), textColor );
}
//Draw the level text again, but with a message saying that you won
void changeTextToWin()
{
	SDL_FreeSurface(text);
	displayName = 1;
	text = TTF_RenderText_Solid(font, "You win!", textColor);
}
//Free all the surfaces loaded
//Todo: Update with all the surfaces
void freeSurface()
{
	SDL_FreeSurface(button);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(background);
	SDL_FreeSurface(selector);
	SDL_FreeSurface(crate);
	SDL_FreeSurface(cursor);
	SDL_FreeSurface(exitTile);
	SDL_FreeSurface(barrierTile);
	int i = 0;
	for (i = 0; i < wallnum-1; i++)
		SDL_FreeSurface(wall[i]);
	for (i = 0; i < 6; i++)
	{
		SDL_FreeSurface(personns[i]);
		SDL_FreeSurface(personew[i]);
	}
	for (i = 0; i < 9; i++)
		SDL_FreeSurface(textBox[i]);
}