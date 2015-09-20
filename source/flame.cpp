#include "sdlFiles.h"
#include "sprites.h"
#include "level.h"
#include "tileEnum.h"
extern Object *player;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
void applyTerrain(int input, int index);
Level* getCurrentLevel();
bool objMove();

extern double delta;

class Flame : Object
{
public:
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
		applyTerrain(m_firefloor, getCurrentLevel()->convertIndex(x, y));
		die();
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
		if (lifeTime < 30.0f)
			return;
		for (int xMod = -1; xMod <= 1; xMod++)
			for (int yMod = -1; yMod <= 1; yMod++)
			{
				if (xMod == 0 && yMod == 0)
					continue;
				Object *tmpObj = getCurrentLevel()->getObject(x + xMod, y + yMod);
				if (tmpObj != NULL)
					tmpObj->heat();
				Terrain *tmpTerrain = getCurrentLevel()->getTerrain(x + xMod, y + yMod);
				if (tmpTerrain != NULL)
					tmpTerrain->heat();
			}
	}
};
SPRITE_STATIONARY(Flame, "gfx/flames.png")
