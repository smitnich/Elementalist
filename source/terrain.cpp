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
bool Floor::requestEntry(Object *other, int dir)
{
	return true;
}
bool Floor::requestExit(Object *other, int dir)
{
	return true;
}
void Floor::onEnter(Object *other)
{
	return;
}
void Floor::onExit(Object *other)
{
	return;
}
void Floor::activate()
{
	return;
}
void Floor::deactivate()
{
	return;
}
void Floor::onCreate()
{
	return;
}
void Floor::onDestroy()
{
	return;
}
bool Floor::isSolid()
{
	return false;
}
void Wall::onEnter(Object *other)
{
	return;
}
void Wall::onExit(Object *other)
{
	return;
}
bool Wall::requestEntry(Object *other, int dir)
{
	return false;
}
bool Wall::requestExit(Object *other, int dir)
{
	return true;
}
void Wall::activate()
{
	return;
}
void Wall::deactivate()
{
	return;
}
void Wall::onCreate()
{
	return;
}
void Wall::onDestroy()
{
	return;
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
void Exit::onExit(Object *other)
{
	return;
}
Exit::Exit()
{
	isTrigger = false;
	this->sprite = exitTile;
}
bool Exit::requestEntry(Object *other, int dir)
{
	return true;
}
bool Exit::requestExit(Object *other, int dir)
{
	return true;
}
void Exit::activate()
{
	return;
}
void Exit::deactivate()
{
	return;
}
void Exit::onCreate()
{
	return;
}
void Exit::onDestroy()
{
	return;
}
bool Exit::isSolid()
{
	return false;
}