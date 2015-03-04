#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
void Duplicator::onEnter(Object *other)
{
	copyObj = other;
}
void Duplicator::onExit(Object *other)
{
	copyObj = NULL;
}
void Duplicator::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void Duplicator::activate()
{

}
Duplicator::Duplicator(int _dir)
{
	dir = _dir;
	sprite = spr_dupe[dir - 1];
	index = 0;
}
