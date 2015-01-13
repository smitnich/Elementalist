#include "gfx.h"
#include "level.h"
#include "objectDef.h"
extern Object *player;
SDL_Rect borderRect[4];
Level *getCurrentLevel();
struct tile
{
	SDL_Surface *contents[NUM_TILE_IMAGES];
	int *xOffset;
	int *yOffset;
	int xWrap;
	int yWrap;
};
void renderTile(int x, int y, int centerX, int centerY, int moveFractionX, int moveFractionY)
{
	int drawX = (x-centerX)*TILE_SIZE+xInitial;
	int drawY = (y-centerY)*TILE_SIZE+yInitial;
	apply_surface(drawX,drawY,0,0,tiles,screen);
}
//Draws the static map features, the objects, the player, and the textbox on the screen
void drawScreen()
{
	Level *currentLevel = getCurrentLevel();
	int x = 0;
	int y = 0;
	int moveFractionX = 0;
	int moveFractionY = 0;
	int moveBy = 0;
	int applyX = 0;
	int applyY = 0;
	int doDir[4] = {0,0,0,0};
	//Determine the value to offset all sprites by
	if (player->objMoveDir == D_UP)
	{
		moveFractionY = player->objMoveFraction*-1;
		doDir[D_UP-1] = 1;
	}
	else if (player->objMoveDir == D_DOWN)
	{
		moveFractionY = player->objMoveFraction;
		doDir[D_DOWN-1] = 1;
	}
	else if (player->objMoveDir == D_LEFT)
	{
		moveFractionX = player->objMoveFraction*-1;
		doDir[D_LEFT-1] = 1;
	}
	else if (player->objMoveDir == D_RIGHT)
	{
		moveFractionX = player->objMoveFraction;
		doDir[D_RIGHT-1] = 1;
	}
	int placeX = -1;
	int placeY = -1;
	if (posX < tilesX+1)
		placeX = 0;
	if (posY < tilesY+1)
		placeY = 0;
	if (posY > MAP_SIZE-tilesY-1)
		placeY = MAP_SIZE-tilesY*2-1;
	if (posX > MAP_SIZE-tilesX-1)
		placeX = MAP_SIZE-tilesX*2-1;
	if (placeX == -1)
		placeX = posX-tilesX;
	if (placeY == -1)
		placeY = posY-tilesY;
	//Don't do the offset if the player is moving towards the edge of the map
	if (placeX == 0||placeX == MAP_SIZE-tilesX*2-1)
	{
		if (!((posX == tilesX && player->objMoveDir == D_RIGHT)||(posX == MAP_SIZE-tilesX-1 && player->objMoveDir == D_LEFT)))
		{
			moveFractionX = 0;
			doDir[D_LEFT-1] = 0;
			doDir[D_RIGHT-1] = 0;
		}
	}
	//Don't do the offset if the player is moving towards the edge of the map
	if (placeY == 0||placeY == MAP_SIZE-tilesY*2-1)
	{
		if (!((posY == tilesY && player->objMoveDir == D_DOWN)||(posY == MAP_SIZE-tilesY-1 && player->objMoveDir == D_UP)))
		{
			moveFractionY = 0;
			doDir[D_UP-1] = 0;
			doDir[D_DOWN-1] = 0;
		}
	}

	//Draw every tile within the players line of sight, including partially visible tiles
	for (x = placeX-doDir[D_LEFT-1]; x < tilesX*2+1+placeX+doDir[D_RIGHT-1]; x++)
	{
		for (y = placeY-doDir[D_UP-1]; y < tilesY*2+1+placeY+doDir[D_DOWN-1]; y++)
		{
			SDL_Surface *toApply[5];
			int applyCount = 0;
			//Move the player if the screen is not moving due to being on the edge of the screen
			if (moveFractionX != 0)
			{
				if (placeX == 0||placeX == MAP_SIZE-tilesX*2-1)
				{
					if (posX == tilesX && player->objMoveDir == D_RIGHT)
						moveBy = moveFractionX;
					else if (posX == MAP_SIZE-tilesX-1 && player->objMoveDir == D_LEFT)
						moveBy = moveFractionX;
					else
						moveBy = 0;
				}
				else
					moveBy = moveFractionX;
				applyX = (x-placeX)*tileSize+xInitial-moveBy;
				applyY = (y-placeY)*tileSize+yInitial;
			}
			//Move the player if the screen is not moving due to being on the edge of the screen
			else if (moveFractionY != 0)
			{
				if (placeY == 0||placeY == MAP_SIZE-tilesY*2-1)
				{
					if (posY == tilesY && player->objMoveDir == D_DOWN)
						moveBy = moveFractionY;
					else if (posY == MAP_SIZE-tilesY-1 && player->objMoveDir == D_UP)
						moveBy = moveFractionY;
					else
						moveBy = 0;
				}
				else
					moveBy = moveFractionY;
				applyX = (x-placeX)*tileSize+xInitial;
				applyY = (y-placeY)*tileSize+yInitial-moveBy;
			}
			else
			{
				applyX = (x-placeX)*tileSize+xInitial;
				applyY = (y-placeY)*tileSize+yInitial;
			}
			//Choose which static feature sprite to use
			toApply[applyCount++] = currentLevel->mapLayer[currentLevel->convertIndex(x,y)]->sprite;
			//Determine whether to cut off the image due to it being on the edge of the screen
			if (x == placeX-doDir[D_LEFT-1])
			{
				//Make sure the value to be drawn is negative
				if (moveFractionX > 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,(tileSize-moveFractionX)*-1,0,toApply[i],screen);
					continue;
				}
				else if (moveFractionX < 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,moveFractionX,0,toApply[i],screen);
					continue;
				}
				//Check if we're also at the top or bottom of the map
				else if (!((y == placeY-doDir[D_UP-1]||y == tilesY*2+placeY+doDir[D_DOWN-1]) && moveFractionY != 0))
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,toApply[i],screen);
					continue;
				}
			}
			//If we're at the east end of the map, cut off the sprite
			if (x == placeX+tilesX*2+doDir[D_RIGHT-1])
			{
				if (moveFractionX > 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,moveFractionX,0,toApply[i],screen);
					continue;
				}
				else if (moveFractionX < 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,tileSize+moveFractionX,0,toApply[i],screen);
					continue;
				}
				else if (!((y == placeY-doDir[D_UP-1]||y == tilesY*2+1+placeY+doDir[D_DOWN-1]-1) && moveFractionY != 0))
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,toApply[i],screen);
					continue;
				}
			}
			if (y == placeY-doDir[D_UP-1])
			{
				if (moveFractionY > 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,0,(tileSize-moveFractionY)*-1,toApply[i],screen);
					continue;
				}
				else if (moveFractionY < 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,0,moveFractionY,toApply[i],screen);
					continue;
				}
				else
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,toApply[i],screen);
					continue;
				}
			}
			if (y == placeY+tilesY*2+doDir[D_DOWN-1])
			{
				if (moveFractionY > 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,0,moveFractionY,toApply[i],screen);
					continue;
				}
				else if (moveFractionY < 0)
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,0,tileSize+moveFractionY,toApply[i],screen);
					continue;
				}
				else
				{
					for (int i = 0; i < applyCount; i++)
						apply_surface(applyX,applyY,toApply[i],screen);
					continue;
				}
			}
			else
				for (int i = 0; i < applyCount; i++)
					apply_surface(applyX,applyY,toApply[i],screen);
		}
	}
	int drawX = 0;
	int drawY = 0;
	int xDisplacement = 0;
	int ydisplacement = 0;
	SDL_Surface *toDraw = NULL;
	drawX = (posX-placeX)*tileSize+xInitial+xDisplacement;
	drawY = (posY-placeY)*tileSize+yInitial+ydisplacement;
	objectDraw();
	//Draw the level name in a text box
	if (displayName == 1)
	{
		doTextBox(posY);
		if (posY < 3)
			apply_surface(xInitial+tileSize*2+getCenter((tilesX+1)*tileSize,text->w),yInitial,text,screen);
		else
			apply_surface(xInitial+getCenter(tilesX*2+1,5)*tileSize+getCenter(tileSize*5,text->w),yInitial+(tilesX+2)*tileSize,text,screen);

	}
	drawSprite(drawX,drawY,toDraw);
	//Draw the mouse if it is within bounds and should be drawn
	if (pointerX > -1 && pointerY > -1 && showCursor == true)
		apply_surface(pointerX,pointerY,cursor,screen);
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