#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
SDL_Surface *spr_freezer[2] = { NULL, NULL };
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
	enabled = !enabled;
	sprite = spr_freezer[enabled];
	if (freezeObj == NULL)
		return;
	int x = freezeObj->x;
	int y = freezeObj->y;
	getCurrentLevel()->getObject(x, y)->freeze();
}
void Freezer::deactivate()
{
	enabled = !enabled;
	sprite = spr_freezer[enabled];
}
Freezer::Freezer(bool _enabled)
{
	freezeObj = NULL;
	enabled = _enabled;
	index = 0;
	//If we're enabled, use the second sprite
	sprite = spr_freezer[enabled];
}
