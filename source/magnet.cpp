#include "objectDef.h"
#include "imageMacros.h"
#include "defs.h"
#include "level.h"

#define MAGNET_ID 1017
#define MAGNET_DIST 10

bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
int reverseDir(int dir);
Level *getCurrentLevel();

class Magnet : public Object {
public:
	//Whether or not this object is activated:
	//ie has been electrocuted this frame
	bool active;
	int dir;
	Magnet(int x, int y) : Object(x,y) {
		active = false;
	}
	Object *clone(int _x, int _y) {
		Object *tmp = new Magnet(_x, _y);
		//Copy all of the contents of this object over to the new one
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
	void electrocute() {
		active = true;
	}
	void doLogic() {
		int xDist = 0;
		int yDist = 0;
		if (active) {
			switch (dir) {
			case D_UP:
				yDist = -MAGNET_DIST;
				break;
			case D_DOWN:
				yDist = MAGNET_DIST;
				break;
			case D_LEFT:
				xDist = -MAGNET_DIST;
				break;
			case D_RIGHT:
				xDist = MAGNET_DIST;
				break;
			}
			pullToward(xDist, yDist);
		}
		active = false;
	}
	bool requestEntry(Object *other, int dir) {
		return false;
	}
	void pullToward(int xDist, int yDist) {
		int xStart = x;
		int yStart = y;
		int xEnd = x+1;
		int yEnd = y+1;
		int xChange = 0;
		int yChange = 0;
		Object *tmp = NULL;
		Level *level = getCurrentLevel();
		if (xDist < 0) {
			xStart = x + xDist;
			xChange = 1;
		}
		else if (xDist > 0) {
			xChange = -1;
			xEnd = x + xDist;
		}
		else if (yDist < 0) {
			yStart = y + yDist;
			yChange = 1;
		}
		else if (yDist > 0) {
			yEnd = y + yDist;
			yChange = -1;
		}
		for (int xIter = xStart; xIter < xEnd; xIter++) {
			for (int yIter = yStart; yIter < yEnd; yIter++) {
				tmp = level->getObject(xIter, yIter);
				if (tmp != NULL && tmp->isMagnetic) {
					if (requestMove(tmp->x, tmp->y, xChange, yChange, tmp)) {
						if (tmp->objMoveDir == D_NONE)
							tmp->startMove(reverseDir(dir), 4);
					}
				}
			}
		}
	}
	OBJECT_DECLARATION(Magnet, 0)
};
SPRITE_STATIONARY(Magnet, NULL)

class MagnetN : public Magnet {
public:
	MagnetN(int x, int y) : Magnet(x, y) {
		dir = D_UP;
	}
	OBJECT_DECLARATION(MagnetN, MAGNET_ID)
};
SPRITE_STATIONARY(MagnetN, "gfx/magnetN.png")

class MagnetS : public Magnet {
public:
	MagnetS(int x, int y) : Magnet(x, y) {
		dir = D_DOWN;
	}
	OBJECT_DECLARATION(MagnetS, MAGNET_ID+1)
};
SPRITE_STATIONARY(MagnetS, "gfx/magnetS.png")

class MagnetW : public Magnet {
public:
	MagnetW(int x, int y) : Magnet(x, y) {
		dir = D_LEFT;
	}
	OBJECT_DECLARATION(MagnetW, MAGNET_ID+2)
};
SPRITE_STATIONARY(MagnetW, "gfx/magnetW.png")

class MagnetE : public Magnet {
public:
	MagnetE(int x, int y) : Magnet(x, y) {
		dir = D_RIGHT;
	}
	OBJECT_DECLARATION(MagnetE, MAGNET_ID+3)
};
SPRITE_STATIONARY(MagnetE, "gfx/magnetE.png")

