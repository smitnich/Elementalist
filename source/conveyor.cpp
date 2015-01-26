#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
#include "objectDef.h"
extern SDL_Surface *conveyorew[6];
extern SDL_Surface *conveyorns[6];
bool Conveyor::requestEntry(Object* other, int objDir)
{
	return true;
}
bool Conveyor::requestExit(Object* other, int objDir)
{
	if (disabled)
		return true;
	if (objDir == D_UP && this->dir == D_DOWN ||
	   (objDir == D_DOWN && this->dir == D_UP) ||
	   (objDir == D_LEFT && this->dir == D_RIGHT) ||
	   (objDir == D_RIGHT && this->dir == D_LEFT))
		return false;
	else
		return true;
}
void Conveyor::activate()
{
	disabled = true;
}
void Conveyor::deactivate()
{
	disabled = false;
}
void Conveyor::onCreate()
{
	return;
}
void Conveyor::onDestroy()
{
	return;
}
void Conveyor::onEnter(Object* other)
{
	if (!disabled)
		other->startMove(dir);
}
void Conveyor::onExit(Object* other)
{
	return;
}
bool Conveyor::isSolid()
{
	return true;
}
Conveyor::Conveyor(int direction)
{
	disabled = false;
	isTrigger = false;
	this->dir = direction;
	switch (this->dir)
	{
	case D_LEFT:
		this->sprite = conveyorew[0];
		break;
	case D_RIGHT:
		this->sprite = conveyorew[3];
		break;
	case D_UP:
		this->sprite = conveyorns[0];
		break;
	case D_DOWN:
		this->sprite = conveyorns[3];
		break;
	}
}