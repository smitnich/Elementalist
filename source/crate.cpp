//Basic movable object, can be moved in a line with other crates
#include "objectDef.h"
#include "objectDef.h"
#include "sprites.h"
#include "level.h"
extern Object *player;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Level* getCurrentLevel();
void objMove();

class Crate : public Object{
public:
	OBJECT_DECLARATION(Crate, 1001)
	bool isMovableBlock()
	{
		return true;
	};
	void electrocute(){

	}
	Crate(int x2, int y2) : Object(x2,y2)
	{
	}
	void doLogic()
	{
		objMove();
	}
	bool requestEntry(Object *other, int dir)
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
		if (requestMove(this->x, this->y, xdir, ydir, this))
		{
			startMove(dir);
			objMoveFraction = other->objMoveFraction;
			return true;
		}
		if (objMoveDir != D_NONE)
			return true;
		return false;
	}	Object *clone(int x, int y);
};

SPRITE_STATIONARY(Crate, "gfx/block.png")

class ColorCrate1 : public Crate
{
public:
	OBJECT_DECLARATION(ColorCrate1, 1011)
	ColorCrate1(int x2, int y2) : Crate(x2,y2)
	{
		
	}
};

SPRITE_STATIONARY(ColorCrate1, "gfx/barrierBlock1.png")

Object* Crate::clone(int _x, int _y)
{
	Object *tmp = new Crate(_x, _y);
	//Copy all of the contents of this object over to the new one
	memcpy(tmp, this, sizeof(*this));
	tmp->x = _x;
	tmp->y = _y;
	return tmp;
}
class HeavyCrate : public Crate{
public:
	OBJECT_DECLARATION(HeavyCrate, 1002)
	HeavyCrate(int x2, int y2) : Crate(x2, y2) {
		isMagnetic = true;
	}
	//Can't be moved in a line
	bool requestEntry(Object *other, int dir)
	{
		int xdir = 0;
		int ydir = 0;
		Object *newLocation = NULL;
		if (dir == D_LEFT)
			xdir = -1;
		else if (dir == D_RIGHT)
			xdir = 1;
		if (dir == D_UP)
			ydir = -1;
		else if (dir == D_DOWN)
			ydir = 1;
		newLocation = getCurrentLevel()->getObject(x + xdir, y + ydir);
		if (newLocation == NULL || newLocation->isMovableBlock() == false)
			return Crate::requestEntry(other, dir);
		else
			return false;
	}
	Object* clone(int _x, int _y)
	{
		Object *tmp = new HeavyCrate(_x, _y);
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
};
SPRITE_STATIONARY(HeavyCrate, "gfx/heavyBlock.png")
class FrozenCrate : Crate {
public:
	OBJECT_DECLARATION(FrozenCrate,1003)
	FrozenCrate(int x, int y) : Crate(x, y){
		this->frozen = true;
	}
};
SPRITE_STATIONARY(FrozenCrate, "gfx/iceBlock.png")