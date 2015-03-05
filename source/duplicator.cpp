#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
void addAssignQueue(Object *in);

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
	if (copyObj == NULL)
		return;
	int x = copyObj->x;
	int y = copyObj->y;
	if (dir == D_UP)
		y -= 1;
	else if (dir == D_DOWN)
		y += 1;
	else if (dir == D_LEFT)
		x -= 1;
	else if (dir == D_RIGHT)
		x += 1;
	if (getCurrentLevel()->getObject(x, y) != NULL)
		return;
	Object *tmp = copyObj->clone(x,y);
	addAssignQueue(tmp);

}
Duplicator::Duplicator(int _dir)
{
	dir = _dir;
	sprite = spr_dupe[dir - 1];
	index = 0;
}
