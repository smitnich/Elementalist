#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
SDL_Surface *spr_blackHole = NULL;
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