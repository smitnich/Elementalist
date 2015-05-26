//Basic movable object, can be moved in a line with other crates
#include "objectDef.h"
#include "base.h"
#include "sprites.h"
#include "level.h"
extern Object *player;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Level* getCurrentLevel();
void objMove();

SPRITE_STATIONARY(Crate, "gfx/block.png")

Crate::Crate(int x2, int y2)
{
	hovering = false;
	numFrames = 1;
	x = x2;
	y = y2;
	objMoveDir = 0;
	objMoveFraction = 0;
	solid = 2;
	faceDir = 0;
	prevMove = D_NONE;
}
ColorCrate1::ColorCrate1(int x2, int y2)
{
	hovering = false;
	numFrames = 1;
	x = x2;
	y = y2;
	objMoveDir = 0;
	objMoveFraction = 0;
	solid = 2;
	faceDir = 0;
	prevMove = D_NONE;
}
SPRITE_STATIONARY(ColorCrate1, "gfx/barrierBlock1.png")
void Crate::doLogic()
{
	objMove();
}
Object* Crate::clone(int _x, int _y)
{
	Object *tmp = new Crate(_x, _y);
	//Copy all of the contents of this object over to the new one
	memcpy(tmp, this, sizeof(*this));
	tmp->x = _x;
	tmp->y = _y;
	return tmp;
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
	if (requestMove(this->x, this->y, xdir, ydir, this))
	{
		startMove(dir);
		return true;
	}
	return false;
}
SPRITE_STATIONARY(HeavyCrate, "gfx/heavyBlock.png")
//Can't be moved in a line
Object* HeavyCrate::clone(int _x, int _y)
{
	Object *tmp = new HeavyCrate(_x, _y);
	memcpy(tmp, this, sizeof(this));
	tmp->x = _x;
	tmp->y = _y;
	return tmp;
}
HeavyCrate::HeavyCrate(int x2, int y2) : Crate(x2,y2)
{

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
	if (getCurrentLevel()->getObject(x + xdir, y + ydir) == NULL && other->isMovableBlock() == false)
		return Crate::requestEntry(other, dir);
	else
		return false;
}
class FrozenCrate : Crate {
public:
	IMAGE_DECLARATION(FrozenCrate,1003)
	FrozenCrate(int x, int y) : Crate(x, y){
		this->frozen = true;
	}
};
SPRITE_STATIONARY(FrozenCrate, "gfx/iceBlock.png")