#include "objectDef.h"
#include "sdlFiles.h"
#include "level.h"
#include "sound.h"
#include "imageMacros.h"

SDL_Surface *spr_pickup = NULL;
SDL_Surface *spr_pickupWall = NULL;

class PickupWall : public Object
{
public:
	OBJECT_DECLARATION(PickupWall, 1010)
	void doLogic()
	{
		if (getCurrentLevel()->pickupCount <= 0)
		{
			playSound(snd_barrierOpen);
			die();
		}
	}
	PickupWall(int _x, int _y) : Object(x,y)
	{
		x = _x;
		y = _y;
		stationary = spr_pickupWall;
	}
	Object* clone(int x, int y)
	{
		return NULL;
	}
	bool requestEntry(Object *other, int dir)
	{
		return false;
	}
};
SPRITE_STATIONARY(PickupWall, "gfx/pickupWall.png")

class Pickup : public Object
{
public:
	OBJECT_DECLARATION(Pickup, 1009)
	//Pickups should be destroyed when any object enters their square
	void onCollision(Object *other, int dir)
	{
		die();
	}
	bool requestEntry(Object *other, int dir)
	{
		return true;
	}
	void die()
	{
		getCurrentLevel()->pickupCount--;
		playSound(snd_crystalBreak);
		Object::die();
	}
	Pickup(int _x, int _y) : Object(x,y)
	{
		hovering = true;
		x = _x;
		y = _y;
		getCurrentLevel()->pickupCount++;
		stationary = spr_pickup;
	}
	Object* clone(int x, int y)
	{
		return NULL;
	}
	bool allowEntry()
	{
		return true;
	}

};

SPRITE_STATIONARY(Pickup, "gfx/pickup.png")