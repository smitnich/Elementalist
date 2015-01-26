#include "terrain.h"
#include "level.h"
void switchLevel(int levelNum);
extern int currentLevelNum;
extern SDL_Surface *wall[];
extern SDL_Surface *tiles;
extern SDL_Surface *exitTile;
extern bool won;
Floor::Floor()
{
	isTrigger = false;
	this->sprite = tiles;
}
void Floor::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	return;
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
void Wall::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	return;
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
void Exit::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	return;
}
