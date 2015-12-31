#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"
#include "sound.h"
#include "level.h"
#include "queues.h"
extern int xInitial, yInitial;
extern Object *player;

unsigned char lookupWall(int index);
int reverseDir(int dir);
extern bool won;
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
Barrier::Barrier(bool _disabled)
{
	defaultState = _disabled;
	totalConnections = numConnectionsActive = 0;
	index = 0;
	disabled = _disabled;
	sprite = barrierTile;
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
		disabled = !defaultState;
	}
}
void Barrier::deactivate()
{
	numConnectionsActive--;
	disabled = defaultState;
}
Bomb::Bomb()
{
	index = 0;
	sprite = spr_bomb;
}
void Bomb::onEnter(Object *other, bool solidFound)
{
	other->toAsh();
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
	return !other->isMovableBlock() && !other->frozen;
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
	{
		playSound(snd_spring);
		return true;
	}
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