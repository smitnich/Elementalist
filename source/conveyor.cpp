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
	if (objDir == D_UP && this->dir == D_DOWN ||
	   (objDir == D_DOWN && this->dir == D_UP) ||
	   (objDir == D_LEFT && this->dir == D_RIGHT) ||
	   (objDir == D_RIGHT && this->dir == D_LEFT))
		return false;
	else
		return true;
}
void Conveyor::OnEnter(Object* other)
{
	other->startMove(dir);
}
void Conveyor::OnExit(Object* other)
{
	return;
}
bool Conveyor::isSolid()
{
	return false;
}
void Conveyor::onActivate()
{
	return;
}
void Conveyor::onDeactivate()
{
	return;
}
void Conveyor::onDestroy()
{
	return;
}
void Conveyor::onCreate()
{
	return;
}
Conveyor::Conveyor(int direction)
{
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
