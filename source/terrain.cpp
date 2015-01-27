#include "terrain.h"
#include "level.h"
void switchLevel(int levelNum);
extern int currentLevelNum;
extern SDL_Surface *wall[];
extern SDL_Surface *tiles;
extern SDL_Surface *exitTile;
extern SDL_Surface *barrierTile;
extern bool won;
Floor::Floor()
{
	isTrigger = false;
	this->sprite = tiles;
}
bool Wall::requestEntry(Object *other, int dir)
{
	return false;
}
bool Wall::isSolid()
{
	return true;
}
Wall::Wall()
{
	isTrigger = false;
	this->sprite = wall[v_wallbase];
}
void Exit::onEnter(Object *other)
{
	if (other->isPlayer)
		won = true;
}
Exit::Exit()
{
	isTrigger = false;
	this->sprite = exitTile;
}
Barrier::Barrier()
{
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
	disabled = true;
}
void Barrier::deactivate()
{
	disabled = false;
}
bool Barrier::isSolid()
{
	return !disabled;
}