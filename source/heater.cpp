#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
SDL_Surface *spr_heater[2] = { NULL, NULL };
extern int xInitial, yInitial;

void Heater::onEnter(Object *other, bool solidFound)
{
	heatObj = other;
	if (enabled)
		heatObj->heat();
}
void Heater::onExit(Object *other)
{
	if (other == heatObj)
		heatObj = NULL;
}
void Heater::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void Heater::activate()
{
	numConnectionsActive++;
	if (numConnectionsActive >= totalConnections) {
		enabled = !enabled;
		sprite = spr_heater[enabled];
		int x = index % getCurrentLevel()->width;
		int y = index / getCurrentLevel()->height;
		getCurrentLevel()->getTerrain(x, y)->heat();
		if (heatObj != NULL)
			heatObj->heat();
	}
}
void Heater::deactivate()
{
	numConnectionsActive--;
	enabled = !enabled;
	sprite = spr_heater[enabled];
}
Heater::Heater(bool _enabled)
{
	totalConnections = numConnectionsActive = 0;
	heatObj = NULL;
	enabled = _enabled;
	index = 0;
	//If we're enabled, use the second sprite
	sprite = spr_heater[enabled];
}
