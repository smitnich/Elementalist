#include "terrain.h"
#include "level.h"

SDL_Surface *spr_flame;
void addTerrainLogic(Terrain *in);
Level *getCurrentLevel();
void addDeleteQueue(Terrain *in);
void removeLogicTerrainRequest(LogicTerrain *in);

extern double delta;

FireFloor::~FireFloor() {
}
FireFloor::FireFloor(int _x, int _y) : LogicTerrain() {
	x = _x;
	y = _y;
	sprite = spr_flame;
	lifeTime = 0;
	timeToLive = 60.0f;
}
bool FireFloor::requestEntry(Object* other, int dir) {
	return true;
}
bool FireFloor::requestExit(Object* other, int dir) {
	return true;
}
void FireFloor::onEnter(Object *other, bool solidFound) {
	other->burn();
}
void FireFloor::doLogic()
{
	lifeTime += delta;
	timeToLive -= delta;
	if (timeToLive < 0)
	{
		addDeleteQueue(this);
		removeLogicTerrainRequest(this);
		return;
	}
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