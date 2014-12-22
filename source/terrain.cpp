#include "terrain.h"
extern SDL_Surface *wall[];
extern SDL_Surface *tiles;
bool Floor::requestEntry(Object *other, int dir)
{
	return true;
}
bool Floor::requestExit(Object *other, int dir)
{
	return true;
}
void Floor::OnEnter(Object* other)
{
	return;
};
void Floor::OnExit(Object* other)
{
	return;
};
bool Floor::isSolid()
{
	return false;
};
void Floor::onActivate()
{
	return;
};
void Floor::onDeactivate()
{
	return;
};
void Floor::onDestroy()
{
	return;		
};
void Floor::onCreate()
{
	return;
};
Floor::Floor()
{
	this->sprite = tiles;
}
bool Wall::requestEntry(Object *other, int dir)
{
	return false;
}
bool Wall::requestExit(Object *other, int dir)
{
	return true;
}
Wall::Wall()
{
	this->sprite = wall[v_wallbase];
}
void Wall::OnEnter(Object* other)
{
	return;
};
void Wall::OnExit(Object* other)
{
	return;
};
bool Wall::isSolid()
{
	return true;
};
void Wall::onActivate()
{
	return;
};
void Wall::onDeactivate()
{
	return;
};
void Wall::onDestroy()
{
	return;		
};
void Wall::onCreate()
{
	return;
};