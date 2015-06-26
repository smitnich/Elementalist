#include "gfx.h"
#include "level.h"
#include "objectDef.h"
#include "debugText.h"
#include "defs.h"
extern Object *player;
SDL_Surface *iceBlock = NULL;
SDL_Rect borderRect[4];
Level *getCurrentLevel();
extern bool playerDead;
void queueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY);
void creationQueueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY);
void renderHighSprites(int moveFractionX, int moveFractionY);
void renderUserInterface(SDL_Surface *screen);
//Draws the static map features, the objects, the player, and the textbox on the screen
void drawScreen()
{
	if (player == NULL)
		exit(0);
	int x, y, xOffset = 0, yOffset = 0, xStart = player->x, yStart = player->y;
	Level *currentLevel = getCurrentLevel();
	bool doDir[4] = { (player->objMoveDir == D_UP),
					   (player->objMoveDir == D_DOWN),
					   (player->objMoveDir == D_LEFT),
					   (player->objMoveDir == D_RIGHT) };
	Terrain *terrain = NULL;
	Object *obj = NULL;
	calculateMoveFraction(player->objMoveDir, player->objMoveFraction, &xOffset, &yOffset);
	if (player->x < NUM_TILES || (player->x == NUM_TILES && doDir[D_RIGHT - 1] != true))
	{
		xOffset = 0;
		xStart = NUM_TILES;
	}
	else if (player->x > currentLevel->width - NUM_TILES-1 || (player->x == currentLevel->width - NUM_TILES-1 && doDir[D_LEFT - 1] != true))
	{
		xOffset = 0;
		xStart = currentLevel->width - NUM_TILES-1;
	}
	if (player->y < NUM_TILES || (player->y == NUM_TILES && doDir[D_DOWN - 1] != true))
	{
		yOffset = 0;
		yStart = NUM_TILES;
	}
	else if (player->y > currentLevel->height - NUM_TILES - 1 || (player->y == currentLevel->height - NUM_TILES - 1 && doDir[D_UP - 1] != true))
	{
		yOffset = 0;
		yStart = currentLevel->height - NUM_TILES-1;
	}
	//Do one tile more than we need so that we get partially drawn tiles correctly
	for (x = -NUM_TILES-1; x <= NUM_TILES+1; x++)
	{
		for (y = -NUM_TILES-1; y <= NUM_TILES+1; y++)
		{
			terrain = currentLevel->getTerrain(xStart + x, yStart + y);
			if (terrain != NULL)
				terrain->draw(screen, x + NUM_TILES, y + NUM_TILES, -xOffset, -yOffset);
		}
	}
	//Now do this again with objects; because objects can be drawn across two tiles, we want to make sure that
	//we don't draw over any of them.
	for (x = -NUM_TILES - 2; x <= NUM_TILES + 2; x++)
	{
		for (y = -NUM_TILES - 2; y <= NUM_TILES + 2; y++)
		{
			obj = currentLevel->getObject(xStart + x, yStart + y);
			if (obj != NULL)
				obj->draw(xOffset,yOffset);
		}
	}
	queueDrawAll(screen, xOffset, yOffset);
	creationQueueDrawAll(screen, xOffset, yOffset);
	renderHighSprites(xOffset, yOffset);
	if (displayName || playerDead)
		doTextBox(player->y);
	//Draw the mouse if it is within bounds and should be drawn
	if (pointerX > -1 && pointerY > -1 && showCursor == true)
		apply_surface(pointerX,pointerY,cursor,screen);
	drawBorders();
	if (debugOn)
		apply_surface(0, 32, renderDebugText(), screen);
	renderUserInterface(screen);
}
void drawWrappedSprite(int x, int y, SDL_Surface* source, SDL_Surface* destination, int xWrap, int yWrap)
{
	//Make sure we're not over the TILE_SIZE
	int i;
	xWrap %= TILE_SIZE;
	yWrap %= TILE_SIZE;
	SDL_Rect *offset[2];
	SDL_Rect *portion[2];
	portion[0] = rectMake(0,0,TILE_SIZE-yWrap,TILE_SIZE-xWrap);
	portion[1] = rectMake((TILE_SIZE-xWrap) % TILE_SIZE, (TILE_SIZE-yWrap) % TILE_SIZE, yWrap, xWrap);
	offset[0] = rectMake(xWrap+x,yWrap+y,TILE_SIZE-yWrap,TILE_SIZE-xWrap);
	offset[1] = rectMake(x,y,yWrap,xWrap);
	for (i = 0; i < 2; i++)
	{
		if (portion[i]->h == 0)
			portion[i]->h = TILE_SIZE;
		if (portion[i]->w == 0)
			portion[i]->w = TILE_SIZE;
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
	yInitial = getCenter(videoSizeY,(tilesY*2+1)*TILE_SIZE);
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
		width = TILE_SIZE;
	if (height == 0)
		height = TILE_SIZE;
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
		portion.x = TILE_SIZE-width;
		offset.x += TILE_SIZE-width;
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
		portion.y = TILE_SIZE-height;
		offset.y += TILE_SIZE-height;
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
	apply_surface(x*TILE_SIZE+xInitial-moveFractionX,y*TILE_SIZE+yInitial-moveFractionY,wall[type],screen);
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
	std::string textName;
	if (startLevelName.compare("") == 0)
		textName.assign(LevelStrings[currentLevelNum].data());
	else
		textName.assign(startLevelName);
	SDL_FreeSurface(text);
	text = TTF_RenderText_Solid( font, textName.c_str(), textColor );
}
void changeTextToDead()
{
	if (text != NULL)
		SDL_FreeSurface(text);
	text = TTF_RenderText_Solid(font, "You died...", textColor);
}
//Draw the level text again, but with a message saying that you won
void changeTextToWin()
{
	if (text != NULL)
		SDL_FreeSurface(text);
	displayName = true;
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