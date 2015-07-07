#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"
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
	within->draw(drawTo,xTile,yTile,xOff,yOff);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void IceFloor::onEnter(Object *other)
{
	if (within->id != m_water)
		within->onEnter(other);
	if (other->prevMove != D_NONE)
		other->startMove(other->prevMove,2);
}
void IceFloor::onExit(Object *other) {
	within->onExit(other);
}
bool IceFloor::requestEntry(Object *other, int dir) {
	return within->requestEntry(other, dir);
}
bool IceFloor::requestExit(Object *other, int dir) {
	return within->requestEntry(other, dir);
}
void IceFloor::whileIn(Object *other) {
	within->whileIn(other);
}
void IceFloor::activate() {
	within->activate();
}
void IceFloor::deactivate() {
	within->deactivate();
}
void IceFloor::freeze() {
	return;
}