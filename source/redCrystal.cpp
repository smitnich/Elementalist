#include "objectDef.h"
#include "tileEnum.h"

class CrystalWall : public Object {
public:
	OBJECT_DECLARATION(CrystalWall, OBJ_CRYSTAL_WALL)
	CrystalWall(int x2, int y2) : Object(x2, y2) {
	}
	Object *clone(int _x, int _y) {
		return NULL;
	}
	bool requestEntry(Object *other, int dir)
	{
		return false;
	}
	void activate()
	{
		die();
	}
};

SPRITE_STATIONARY(CrystalWall, "gfx/redCrystalWall.png")
