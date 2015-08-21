#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"

Terrain* addTerrainChange(int index, Terrain *changeTo);
void removeTerrain(Terrain *terrain);

SDL_Surface *iceFloor = NULL;
IceFloor::~IceFloor() {
}
IceFloor::IceFloor()
{
	sprite = iceFloor;
}
void IceFloor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff){
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, sprite, drawTo);
}
void IceFloor::onEnter(Object *other)
{
	if (other->prevMove != D_NONE)
		other->startMove(other->prevMove,2);
}
void IceFloor::freeze() {
	return;
}
void IceFloor::heat() {
	removeTerrain(this);
}