#include "sdlFiles.h"
#include "sprites.h"
#include "level.h"
#include "tileEnum.h"
#include "terrain.h"
#include "object.h"
#include "main.h"
#include "imageMacros.h"

extern Object *player;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Level* getCurrentLevel();

extern double delta;

class Flame : Object
{
public:
	int startTime = 0;
	OBJECT_DECLARATION(Flame,1012)
	Flame(int x2, int y2) : Object(x2,y2)
	{
		hovering = false;
		numFrames = 1;
		x = x2;
		y = y2;
		objMoveDir = 0;
		objMoveFraction = 0;
		faceDir = 0;
		prevMove = D_NONE;
		startTime = getTicks();
	}
	Object* clone(int _x, int _y)
	{
		Object *tmp = new Flame(_x, _y);
		//Copy all of the contents of this object over to the new one
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
	void doLogic()
	{
		applyTerrain(m_firefloor, getCurrentLevel()->convertIndex(x, y));
		die();
	}
};
SPRITE_STATIONARY(Flame, "gfx/flames.png")