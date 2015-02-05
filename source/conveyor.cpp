﻿#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
#include "objectDef.h"
void drawWrappedSprite(int x, int y, SDL_Surface* source, SDL_Surface* destination, int xWrap, int yWrap);
extern SDL_Surface *conveyorew[6];
extern SDL_Surface *conveyorns[6];
extern double delta, fpsModifier;
extern int conveyorSpeed;
bool Conveyor::requestEntry(Object* other, int objDir)
{
	return true;
}
bool Conveyor::requestExit(Object* other, int objDir)
{
	if (disabled)
		return true;
	if (objDir == D_UP && this->dir == D_DOWN ||
	   (objDir == D_DOWN && this->dir == D_UP) ||
	   (objDir == D_LEFT && this->dir == D_RIGHT) ||
	   (objDir == D_RIGHT && this->dir == D_LEFT))
		return false;
	else
		return true;
}
void Conveyor::activate()
{
	disabled = true;
}
void Conveyor::deactivate()
{
	disabled = false;
	if (lastEntered != NULL)
		lastEntered->startMove(dir);
}
void Conveyor::onEnter(Object* other)
{
	if (!disabled)
		other->startMove(dir);
	lastEntered = other;
}
void Conveyor::onExit(Object* other)
{
	lastEntered = NULL;
}
bool Conveyor::isSolid()
{
	return true;
}
Conveyor::Conveyor(int direction)
{
	index = 0;
	lastEntered = NULL;
	moveFraction = 0;
	disabled = false;
	isTrigger = false;
	this->dir = direction;
	switch (this->dir)
	{
	case D_LEFT:
		this->sprite = conveyorew[0];
		break;
	case D_RIGHT:
		this->sprite = conveyorew[3];
		break;
	case D_UP:
		this->sprite = conveyorns[0];
		break;
	case D_DOWN:
		this->sprite = conveyorns[3];
		break;
	}
}
void Conveyor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xWrap = 0;
	int yWrap = 0;
	if (!disabled || moveFraction >= 1)
		moveFraction += delta*fpsModifier;
	if (moveFraction >= TILE_SIZE)
		moveFraction -= TILE_SIZE;
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	switch (dir)
	{
	case D_UP:
		yWrap = TILE_SIZE-moveFraction;
		break;
	case D_DOWN:
		yWrap = moveFraction;
		break;
	case D_LEFT:
		xWrap = TILE_SIZE-moveFraction;
		break;
	case D_RIGHT:
		xWrap = moveFraction;
		break;
	}
	drawWrappedSprite(xStart, yStart, sprite, drawTo,xWrap,yWrap);
}