#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
SDL_Surface *spr_blackHole = NULL;
SDL_Surface *spr_electricFloor = NULL;
extern SDL_Surface *tiles;

BlackHole::BlackHole()
{
	sprite = spr_blackHole;
}
void BlackHole::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void BlackHole::onEnter(Object *other)
{
	other->die();
}
void ElectricFloor::onEnter(Object *other)
{
	other->electrocute();
}
void ElectricFloor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	if (enabled)
		apply_surface(xStart, yStart, sprite, drawTo);
}
void ElectricFloor::activate()
{
	enabled = !enabled;
}
void ElectricFloor::deactivate()
{
	enabled = !enabled;
}
ElectricFloor::ElectricFloor(bool _enabled)
{
	sprite = spr_electricFloor;
	enabled = _enabled;
}