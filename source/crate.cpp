//Basic movable object, can be moved in a line with other crates
#include "objectDef.h"
#include "base.h"
#include "sprites.h"
#include "level.h"
extern Object *player;
extern int pressureCount;
void moveLine(int, int, int);
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Level* getCurrentLevel();
void objMove();
	Crate::Crate()
	{
	}
	Crate::Crate(int x2, int y2)
	{
		this->stationary = crate;
		this->numFrames = 1;
		for (int i = 0; i < 6; i++)
		{
			this->spriteew[i] = crate;
			this->spritens[i] = crate;
		}
		this->x = x2;
		this->y = y2;
		this->objMoveDir = 0;
		this->objMoveFraction = 0;
		this->solid = 2;
		this->faceDir = 0;
	}
	void Crate::doLogic()
	{
		Movable::specialLogic();
	}
	bool Crate::requestEntry(Object *other, int dir)
	{
		int xdir = 0;
		int ydir = 0;
		if (dir == D_LEFT)
			xdir = -1;
		else if (dir == D_RIGHT)
			xdir = 1;
		if (dir == D_UP)
			ydir = -1;
		else if (dir == D_DOWN)
			ydir = 1;
		if (requestMove(this->x, this->y, xdir, ydir, other))
		{
			startMove(dir);
			return true;
		}
		return false;
	}
//Can't be moved in a line
	HeavyCrate::HeavyCrate() : Crate()
	{
	}
	HeavyCrate::HeavyCrate(int x2, int y2)
	{
		this->stationary = heavyCrate;
		this->x = x2;
		this->y = y2;
		this->objMoveDir = 0;
		this->objMoveFraction = 0;
		this->solid = 4;
		this->faceDir = 0;
		for (int i = 0; i < 6; i++)
		{
			this->spriteew[i] = heavyCrate;
			this->spritens[i] = heavyCrate;
		}
	}
	bool HeavyCrate::requestEntry(Object *other, int dir)
	{
		int xdir = 0;
		int ydir = 0;
		if (dir == D_LEFT)
			xdir = -1;
		else if (dir == D_RIGHT)
			xdir = 1;
		if (dir == D_UP)
			ydir = -1;
		else if (dir == D_DOWN)
			ydir = 1;
		if (getCurrentLevel()->getObject(x + xdir, y + ydir) == NULL)
			return Crate::requestEntry(other, dir);
		else
			return false;
	}