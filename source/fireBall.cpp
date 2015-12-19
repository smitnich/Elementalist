#include "objectDef.h"
#include "defs.h"
#include "level.h"

#define FIREBALL_ID 1021

void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY);

class FireBall : public Object {
public:
		OBJECT_DECLARATION(FireBall, 0)
		int prevMove;
		FireBall(int x2, int y2) : Object(x2, y2) {
		hovering = true;
		x = x2;
		y = y2;
		objMoveDir = D_LEFT;
		objMoveFraction = 0;
		faceDir = D_LEFT;
		prevMove = D_NONE;
	}
	Object *clone(int _x, int _y) {
		Object *tmp = new FireBall(_x, _y);
		//Copy all of the contents of this object over to the new one
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
	void doLogic() {
		// Heat only the terrain that the fireball is currently on
		getCurrentLevel()->getTerrain(x, y)->heat();
		// But heat all nearby objects
		Object *tmpObj = getCurrentLevel()->getObject(x + 1, y);
		if (tmpObj != NULL)
			tmpObj->heat();
		tmpObj = getCurrentLevel()->getObject(x - 1, y);
		if (tmpObj != NULL)
			tmpObj->heat();
		tmpObj = getCurrentLevel()->getObject(x, y - 1);
		if (tmpObj != NULL)
			tmpObj->heat();
		tmpObj = getCurrentLevel()->getObject(x, y + 1);
		if (tmpObj != NULL)
			tmpObj->heat();
		objMove();
		if (objMoveFraction == 0.0f) {
			preferRightTurn();
		}
	}
	void freeze() {
		die();
	}
	void heat() {
		return;
	}
	void burn()
	{
		return;
	}
	void onCollision(Object *other, int dir){
		other->burn();
	}
};
SPRITE_MOVING(FireBall)
class FireBallW : public FireBall {
public:
	OBJECT_DECLARATION(FireBallW, FIREBALL_ID+3)
		FireBallW(int x2, int y2) : FireBall(x2, y2) {
		startMove(D_LEFT, 1);
	}
};
SPRITE_MOVING(FireBallW)
class FireBallE : public FireBall {
public:
	OBJECT_DECLARATION(FireBallE, FIREBALL_ID)
		FireBallE(int x2, int y2) : FireBall(x2, y2) {
		startMove(D_RIGHT, 1);
	}
};
SPRITE_MOVING(FireBallE)
class FireBallN : public FireBall {
public:
	OBJECT_DECLARATION(FireBallN, FIREBALL_ID+1)
		FireBallN(int x2, int y2) : FireBall(x2, y2) {
		startMove(D_UP, 1);
	}
};
SPRITE_MOVING(FireBallN)
class FireBallS : public FireBall {
public:
	OBJECT_DECLARATION(FireBallS, FIREBALL_ID+2)
		FireBallS(int x2, int y2) : FireBall(x2, y2) {
		startMove(D_DOWN, 1);
	}
};
SPRITE_MOVING(FireBallS)
#define ALL_IMAGES { { "gfx/elementals/fireballn.png", "gfx/elementals/fireballn.png", "gfx/elementals/fireballn.png" },\
{ "gfx/elementals/fireballw.png", "gfx/elementals/fireballw.png", "gfx/elementals/fireballw.png" },\
{ "gfx/elementals/fireballs.png", "gfx/elementals/fireballs.png", "gfx/elementals/fireballs.png"},\
{ "gfx/elementals/fireballe.png", "gfx/elementals/fireballe.png", "gfx/elementals/fireballe.png" }\
};
const char *FireBall::imageNames[4][3] = ALL_IMAGES;
const char *FireBallW::imageNames[4][3] = ALL_IMAGES;
const char *FireBallS::imageNames[4][3] = ALL_IMAGES;
const char *FireBallE::imageNames[4][3] = ALL_IMAGES;
const char *FireBallN::imageNames[4][3] = ALL_IMAGES;
