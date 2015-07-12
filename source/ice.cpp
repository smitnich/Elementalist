#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"

Terrain* addTerrainChange(int index, Terrain *changeTo);

SDL_Surface *iceFloor = NULL;
IceFloor::~IceFloor() {
	if (within != NULL)
		delete(within);
}
IceFloor::IceFloor(Terrain *within)
{
	this->within = within;
	sprite = iceFloor;
}
void IceFloor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff){
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	if (within != NULL)
		within->draw(drawTo,xTile,yTile,xOff,yOff);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void IceFloor::onEnter(Object *other)
{
	if (within != NULL && within->id != m_water)
		within->onEnter(other);
	if (other->prevMove != D_NONE)
		other->startMove(other->prevMove,2);
}
void IceFloor::onExit(Object *other) {
	if (within != NULL)
		within->onExit(other);
}
bool IceFloor::requestEntry(Object *other, int dir) {
	return (within != NULL && within->requestEntry(other, dir));
}
bool IceFloor::requestExit(Object *other, int dir) {
	return (within != NULL && within->requestEntry(other, dir));
}
void IceFloor::whileIn(Object *other) {
	if (within != NULL)
		within->whileIn(other);
}
void IceFloor::activate() {
	if (within != NULL)
		within->activate();
}
void IceFloor::deactivate() {
	if (within != NULL)
		within->deactivate();
}
void IceFloor::freeze() {
	return;
}
void IceFloor::heat() {
	addTerrainChange(index, within);
	within = NULL;
}