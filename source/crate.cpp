//Basic movable object, can be moved in a line with other crates
#include "objectDef.h"
#include "sprites.h"
#include "level.h"
#include "tileEnum.h"
#include "queues.h"
#include "player.h"
#include "terrain.h"
#include "object.h"

bool requestMove(int x, int y, int xChange, int yChange, Object* obj);

extern double delta;
extern Mix_Chunk* snd_explode;

class Crate : public Object {
public:
	OBJECT_DECLARATION(Crate, 1001)
	bool isMovableBlock()
	{
		return true;
	}
	void electrocute(){

	}
	Crate(int x2, int y2) : Object(x2,y2)
	{
	}
	void doLogic()
	{
		objMove();
	}
	void onCollision(Object *other, int dir) {
		startMove(dir, 2);
		if (other->objMoveDir != D_NONE)
			objMoveFraction = other->objMoveFraction;
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
		if (requestMove(x,y,xdir,ydir,this)){
			return true;
		}
		if (objMoveDir != D_NONE)
			return true;
		return false;
	}
	void heat() {
		Object *newObj = addSwitchQueue(this, 1012);
		newObj->setTimeToLive(60.0);
	}
	Object *clone(int x, int y);
};

SPRITE_STATIONARY(Crate, "gfx/block.png")

class ColorCrate1 : public Crate
{
public:
	OBJECT_DECLARATION(ColorCrate1, 1011)
	ColorCrate1(int x2, int y2) : Crate(x2,y2)
	{
		
	}
	void heat() {
		return;
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
		if (other->isMovableBlock())
			return false;
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
	void heat(Object *heatObj = NULL) {
		return;
	}
};
SPRITE_STATIONARY(HeavyCrate, "gfx/heavyBlock.png")
class FrozenCrate : Crate {
public:
	OBJECT_DECLARATION(FrozenCrate,1003)
	FrozenCrate(int x, int y) : Crate(x, y){
		this->frozen = true;
	}
	void freeze() {
		return;
	}
	void heat(Object *heatObj = NULL) {
		die();
	}
};
SPRITE_STATIONARY(FrozenCrate, "gfx/iceBlock.png")

class OilBarrel : Crate {
public:
	OBJECT_DECLARATION(OilBarrel,1025)
		OilBarrel(int x, int y) : Crate(x, y) {
	}
	void doLogic() {
		bool moveFinished = objMove();
		if (getCurrentLevel()->getTerrain(x, y)->id == m_water) {
			if (getCurrentLevel()->getTerrain(x, y - 1)->id == m_water) {
				applyTerrain(m_oilspill, getCurrentLevel()->convertIndex(x, y - 1));
			}
			if (getCurrentLevel()->getTerrain(x-1, y)->id == m_water) {
				applyTerrain(m_oilspill, getCurrentLevel()->convertIndex(x-1, y));
			}
			if (getCurrentLevel()->getTerrain(x, y + 1)->id == m_water) {
				applyTerrain(m_oilspill, getCurrentLevel()->convertIndex(x, y + 1));
			}
			if (getCurrentLevel()->getTerrain(x + 1, y)->id == m_water) {
				applyTerrain(m_oilspill, getCurrentLevel()->convertIndex(x + 1, y));
			}
			return;
		}
		if (!frozen && moveFinished) {
			applyTerrain(m_oilspill, getCurrentLevel()->convertIndex(x, y));
		}
	}
	void heat() {
		Object *flame = addSwitchQueue(this, OBJ_FLAME);
		flame->setTimeToLive(120.0f);
	}
};
SPRITE_STATIONARY(OilBarrel, "gfx/oilBarrel.png")

#define BOULDER_ID 1026

class Boulder : Crate {
public:
	bool moving;
	OBJECT_DECLARATION(Boulder, BOULDER_ID)
		Boulder(int x, int y) : Crate(x, y) {
		moving = false;
	}
	//Objects that move in some way
	bool objMove()
	{
		int checkX = 0;
		int checkY = 0;
		if (objMoveDir == D_NONE)
			return false;
		if (objMoveFraction == 0.0) {
			prevMove = objMoveDir;
		}
		if (objMoveDir == D_LEFT)
			checkX = -1;
		else if (objMoveDir == D_UP)
			checkY = -1;
		else if (objMoveDir == D_DOWN)
			checkY = 1;
		else if (objMoveDir == D_RIGHT)
			checkX = 1;
		Object *other = getCurrentLevel()->getObject(x + checkX, y + checkY);
		if (other != NULL) {
			other->die();
			onCollision(other, objMoveDir);
		}
		//Make sure that we can move where we're trying to
		if (getCurrentLevel()->getTerrain(x, y)->requestExit(this, objMoveDir) && requestMove(x, y, checkX, checkY, this))
		{
			objMoveFraction += tempSpeed*delta;
		}
		//Otherwise bounce back
		else
		{
			objMoveFraction -= tempSpeed*delta;
			if (objMoveFraction <= 0)
			{
				objMoveDir = D_NONE;
				objMoveFraction = 0;
			}
		}
		/*If the object has reached the next tile, reset its move variable
		and increment its position variable*/
		if (objMoveFraction >= TILE_SIZE)
		{
			Level *lev = getCurrentLevel();
			if (lev->getObject(x, y) == this)
				lev->assignObject(x, y, NULL);
			prevMove = objMoveDir;
			if (objMoveDir == D_LEFT)
				checkX = -1;
			else if (objMoveDir == D_RIGHT)
				checkX = 1;
			else if (objMoveDir == D_UP)
				checkY = -1;
			else if (objMoveDir == D_DOWN)
				checkY = 1;
			if (this->frozen == false)
				objMoveDir = D_NONE;
			objMoveFraction = 0;
			addMoveRequest(this, x, y, checkX, checkY);
			return true;
		}
		return false;
	}
	void doLogic() {
		if (objMove()) {
			if (!startMove(prevMove, 2)) {
				moving = false;
			}
		}
	}
	void onCollision(Object *other, int dir) {
		if (!moving) {
			if (startMove(dir, 2)) {
				objMoveFraction = other->objMoveFraction;
				moving = true;
			}
		}
		else {
			if (other->objMoveDir != dir)
			other->die();
		}
	}
};

SPRITE_STATIONARY(Boulder, "gfx/elementals/boulder.png")