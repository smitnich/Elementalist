#include "objectDef.h"
#include "defs.h"

class IceBall : public Object {
public:
	OBJECT_DECLARATION(IceBall, 0)
	int lastMoveDir;
	IceBall(int x2, int y2) : Object(x2, y2) {
		hovering = true;
		x = x2;
		y = y2;
		objMoveDir = D_LEFT;
		objMoveFraction = 0;
		faceDir = D_LEFT;
		prevMove = D_NONE;
		lastMoveDir = D_NONE;
	}
	Object *clone(int _x, int _y) {
		Object *tmp = new IceBall(_x, _y);
		//Copy all of the contents of this object over to the new one
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
	void doLogic() {
		objMove();
		if (objMoveFraction == D_NONE) {
			preferLeftTurn();
		}
	}
	void freeze() {
		return;
	}
	void heat() {
		die();
	}
	void onEnterStart(Object *other, int dir){
		other->freeze();
	}
};
SPRITE_MOVING(IceBall)
class IceBallW : public IceBall {
public:
	OBJECT_DECLARATION(IceBallW, 1008)
	IceBallW(int x2, int y2) : IceBall(x2, y2) {
		startMove(D_LEFT, 1);
	}
};
SPRITE_MOVING(IceBallW)
class IceBallE : public IceBall {
public:
	OBJECT_DECLARATION(IceBallE, 1005)
		IceBallE(int x2, int y2) : IceBall(x2, y2) {
		startMove(D_RIGHT, 1);
	}
};
SPRITE_MOVING(IceBallE)
class IceBallN : public IceBall {
public:
	OBJECT_DECLARATION(IceBallN, 1006)
		IceBallN(int x2, int y2) : IceBall(x2, y2) {
		startMove(D_UP, 1);
	}
};
SPRITE_MOVING(IceBallN)
class IceBallS : public IceBall {
public:
	OBJECT_DECLARATION(IceBallS, 1007)
		IceBallS(int x2, int y2) : IceBall(x2, y2) {
		startMove(D_DOWN, 1);
	}
};
SPRITE_MOVING(IceBallS)
#define ALL_IMAGES { { "gfx/elementals/iceballn1.png", "gfx/elementals/iceballn2.png", "gfx/elementals/iceballn3.png" },\
{ "gfx/elementals/iceballw1.png", "gfx/elementals/iceballw2.png", "gfx/elementals/iceballw3.png" },\
{ "gfx/elementals/iceballs1.png", "gfx/elementals/iceballs2.png", "gfx/elementals/iceballs3.png"},\
{ "gfx/elementals/iceballe1.png", "gfx/elementals/iceballe1.png", "gfx/elementals/iceballe1.png" }\
};
const char *IceBall::imageNames[4][3] = ALL_IMAGES;
const char *IceBallW::imageNames[4][3] = ALL_IMAGES;
const char *IceBallS::imageNames[4][3] = ALL_IMAGES;
const char *IceBallE::imageNames[4][3] = ALL_IMAGES;
const char *IceBallN::imageNames[4][3] = ALL_IMAGES;
