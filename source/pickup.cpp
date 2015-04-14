#include "objectDef.h"
#include "sdlFiles.h"
#include "level.h"
SDL_Surface *spr_pickup = NULL;
SDL_Surface *spr_pickupWall = NULL;

SPRITE_STATIONARY(Pickup, "gfx/pickup.png")
SPRITE_STATIONARY(PickupWall, "gfx/pickupWall.png")

//Pickups should be destroyed when any object enters their square
bool Pickup::requestEntry(Object *other, int dir)
{
	die();
	return true;
}
void Pickup::die()
{
	getCurrentLevel()->pickupCount--;
	delete this;
}
Pickup::Pickup(int _x, int _y)
{
	hovering = true;
	x = _x;
	y = _y;
	getCurrentLevel()->pickupCount++;
	stationary = spr_pickup;
}
Object* Pickup::clone(int x, int y)
{
	return NULL;
}
bool PickupWall::requestEntry(Object *other, int dir)
{
	return false;
}
void PickupWall::doLogic()
{
	if (getCurrentLevel()->pickupCount <= 0)
		delete this;
}
PickupWall::PickupWall(int _x, int _y)
{
	x = _x;
	y = _y;
	stationary = spr_pickupWall;
}
Object* PickupWall::clone(int x, int y)
{
	return NULL;
}