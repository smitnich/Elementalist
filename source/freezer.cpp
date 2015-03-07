#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
void addTransitQueue(Object *in);

void Freezer::onEnter(Object *other)
{
	freezeObj = other;
	if (enabled)
		freezeObj->freeze();
}
void Freezer::onExit(Object *other)
{
	if (other == freezeObj)
		freezeObj = NULL;
}
void Freezer::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void Freezer::activate()
{
	if (freezeObj == NULL)
		return;
	int x = freezeObj->x;
	int y = freezeObj->y;
	getCurrentLevel()->getObject(x, y)->freeze();
	enabled = true;
}
void Freezer::deactivate()
{
	enabled = false;
}
Freezer::Freezer(bool _enabled)
{
	freezeObj = NULL;
	enabled = _enabled;
	sprite = spr_freezer;
	index = 0;
}
