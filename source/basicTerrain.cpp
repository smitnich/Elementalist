#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"
#include "sound.h"
#include "level.h"
extern int xInitial, yInitial;


unsigned char lookupWall(int index);
int reverseDir(int dir);
extern bool won;
int colorBlockIds[BARRIER_TYPES] = { OBJ_YELLOW_BLOCK };
Terrain *addTerrainChange(int index, Terrain *newTerrain);
Terrain *addTerrainChange(int index, int newIndex);

Floor::Floor()
{
	totalConnections = 0;
	numConnectionsActive = 0;
	index = 0;
	isTrigger = false;
	this->sprite = tiles;
}
bool Wall::requestEntry(Object *other, int dir)
{
	return false;
}
Wall::Wall(int _index)
{
	index = _index;
	isTrigger = false;
	if (index < 0)
		sprite = wall[0];
	else
		sprite = wall[lookupWall(index)];
}
void Exit::onEnter(Object *other, bool solidFound)
{
	if (other->isPlayer)
		won = true;
}
Exit::Exit()
{
	totalConnections = numConnectionsActive = 0;
	index = 0;
	isTrigger = false;
	this->sprite = exitTile;
}
Barrier::Barrier()
{
	totalConnections = numConnectionsActive = 0;
	index = 0;
	disabled = false;
	this->sprite = barrierTile;
}
void Barrier::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	if (!disabled)
		apply_surface(xStart, yStart, sprite, drawTo);
}
bool Barrier::requestEntry(Object *other, int dir)
{
	return disabled;
}
void Barrier::activate()
{
	numConnectionsActive++;
	if (numConnectionsActive >= totalConnections) {
		disabled = true;
	}
}
void Barrier::deactivate()
{
	numConnectionsActive--;
	disabled = false;
}
void ColorBarrier::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
ColorBarrier::ColorBarrier(int type)
{
	index = 0;
	colorType = type;
	sprite = spr_colorBarrier[type];
}
bool ColorBarrier::requestEntry(Object *other, int dir)
{
	//Need to check if the other object is a block of the right
	//color and delete this terrain as well as the block
	if (other->id == colorBlockIds[this->colorType])
	{
		return true;
	}
	return false;
}
void ColorBarrier::onEnter(Object *other, bool solidFound)
{
	addTerrainChange(index, m_floor);
	other->die();
}
Bomb::Bomb()
{
	index = 0;
	sprite = spr_bomb;
}
void Bomb::onEnter(Object *other, bool solidFound)
{
	other->die();
	addTerrainChange(index, m_floor);
	playSound(snd_explode);
}
void Bomb::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
RaisedFloor::RaisedFloor()
{
	sprite = spr_raisedFloor;
}
bool RaisedFloor::requestEntry(Object *other, int dir)
{
	if (other == NULL)
		return true;
	return !other->isMovableBlock();
}
BounceWall::BounceWall() {
	sprite = spr_bounceWall;
}
void BounceWall::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}

bool BounceWall::requestEntry(Object *other, int dir) {
	Terrain *terrain = getCurrentLevel()->getTerrain(other->x, other->y);
	other->prevMove = reverseDir(dir);
	terrain->onEnter(other);
	if (other->objMoveDir != dir)
		return true;
	else
		return false;
}
RisingWall::RisingWall(int _index) {
	sprite = spr_risingWall;
	index = _index;
}
void RisingWall::onExit(Object *other) {
	addTerrainChange(index, m_defaultWall);
}