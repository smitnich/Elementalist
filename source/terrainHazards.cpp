#include "sdlFiles.h"
#include "terrain.h"
SDL_Surface *spr_blackHole = NULL;
SDL_Surface *spr_electricFloor[2] = { NULL, NULL };
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
void BlackHole::onEnter(Object *other, bool solidFound)
{
	other->die();
}
void ElectricFloor::onEnter(Object *other, bool solidFound)
{
	if (enabled)
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
	numConnectionsActive++;
	if (numConnectionsActive >= totalConnections) {
		enabled = !enabled;
		sprite = spr_electricFloor[enabled];
	}
}
void ElectricFloor::deactivate()
{
	numConnectionsActive--;
	if (numConnectionsActive == (totalConnections - 1)) {
		enabled = !enabled;
		sprite = spr_electricFloor[enabled];
	}
}
ElectricFloor::ElectricFloor(bool _enabled)
{
	totalConnections = numConnectionsActive = 0;
	sprite = spr_electricFloor[_enabled];
	enabled = _enabled;
}
void ElectricFloor::whileIn(Object *other) {
	onEnter(other);
}