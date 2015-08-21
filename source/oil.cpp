#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"

void removeTerrain(Terrain *terrain);
Terrain *addTerrainChange(int index, int changeTo);
Terrain *addTerrainChange(int index, Terrain *changeTo);

Object *objectInit(unsigned int id, int idx);

SDL_Surface *spr_oilFloor = NULL;
OilFloor::~OilFloor() {
}
OilFloor::OilFloor()
{
	objWithin = false;
	heated = false;
	sprite = spr_oilFloor;
	solid = false;
}
void OilFloor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff){
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, sprite, drawTo);
}
void OilFloor::onEnter(Object *other)
{
	objWithin = true;
}
void OilFloor::onExit(Object *other) {
	objWithin = false;
}
bool OilFloor::requestEntry(Object *other, int dir) {
	return true;
}
bool OilFloor::requestExit(Object *other, int dir) {
	return true;
}
void OilFloor::heat() {
	removeTerrain(this);
	objectInit(1012, index)->setTimeToLive(60.0);
}