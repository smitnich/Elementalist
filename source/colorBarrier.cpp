#include "objectDef.h"
#include "imageMacros.h"
#include "sdlFiles.h"
#include "tileEnum.h"

#define BARRIER_TYPES 4

static int colorBlockIds[BARRIER_TYPES] = { OBJ_BARRIER_BLOCK_1, OBJ_BARRIER_BLOCK_2, OBJ_BARRIER_BLOCK_3, OBJ_BARRIER_BLOCK_4 };

class ColorBarrier1 : public Object {
public:
	OBJECT_DECLARATION(ColorBarrier1, OBJ_BARRIER_1)
	ColorBarrier1(int _x, int _y) : Object(_x, _y) {}
	bool requestEntry(Object *other, int dir)
	{
		return (other->id == colorBlockIds[id - OBJ_BARRIER_1]);
	}
	void onEnter(Object *other, int xChange, int yChange)
	{
		other->die();
		die();
	}
	Object *clone(int _x, int _y)
	{
		return NULL;
	}
	bool allowEntry()
	{
		return true;
	}
};

SPRITE_STATIONARY(ColorBarrier1, "gfx/wall/colorBarrier1.png")

class ColorBarrier2 : public ColorBarrier1 {
public:
	OBJECT_DECLARATION(ColorBarrier2, OBJ_BARRIER_2)
	ColorBarrier2(int _x, int _y) : ColorBarrier1(_x, _y) {}
	Object *clone(int _x, int _y)
	{
		return NULL;
	}
};

SPRITE_STATIONARY(ColorBarrier2, "gfx/wall/colorBarrier2.png")

class ColorBarrier3 : public ColorBarrier1 {
public:
	OBJECT_DECLARATION(ColorBarrier3, OBJ_BARRIER_3)
	ColorBarrier3(int _x, int _y) : ColorBarrier1(_x, _y) {}
	Object *clone(int _x, int _y)
	{
		return NULL;
	}
};

SPRITE_STATIONARY(ColorBarrier3, "gfx/wall/colorBarrier3.png")

class ColorBarrier4 : public ColorBarrier1 {
public:
	OBJECT_DECLARATION(ColorBarrier4, OBJ_BARRIER_4)
	ColorBarrier4(int _x, int _y) : ColorBarrier1(_x, _y) {}
	Object *clone(int _x, int _y)
	{
		return NULL;
	}
};

SPRITE_STATIONARY(ColorBarrier4, "gfx/wall/colorBarrier4.png")