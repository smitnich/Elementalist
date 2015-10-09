#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
SDL_Surface *spr_freezer[2] = { NULL, NULL };
extern int xInitial, yInitial;

void Freezer::onEnter(Object *other, bool solidFound)
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
	numConnectionsActive++;
	if (numConnectionsActive >= totalConnections) {
		enabled = !enabled;
		sprite = spr_freezer[enabled];
		if (freezeObj == NULL)
			return;
		int x = freezeObj->x;
		int y = freezeObj->y;
		getCurrentLevel()->getObject(x, y)->freeze();
	}
}
void Freezer::deactivate()
{
	numConnectionsActive--;
	enabled = !enabled;
	sprite = spr_freezer[enabled];
}
Freezer::Freezer(bool _enabled)
{
	totalConnections = numConnectionsActive = 0;
	freezeObj = NULL;
	enabled = _enabled;
	index = 0;
	//If we're enabled, use the second sprite
	sprite = spr_freezer[enabled];
}
