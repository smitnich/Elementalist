#include "objectDef.h"
#include "terrain.h"
#include "level.h"
extern int pressureCount;
extern int totalPressureCount;
extern class Level *allLevels[MAX_LEVEL];
Level *getCurrentLevel();

//Checks the space given a coordinate to start with
//and a direction to go in
bool requestMove(int x, int y, int xChange, int yChange, Object* obj)
{
	if (x+xChange < 0 || x+xChange >= MAP_SIZE || y+yChange < 0 || y+yChange >= MAP_SIZE)
		return false;
	Level *level = getCurrentLevel();
	int index = level->convertIndex(x+xChange,y+yChange);
	Terrain *terrain = level->mapLayer.at(index);
	Object *otherObj = level->objectLayer[level->convertIndex(x+xChange,y+yChange)];
	if (otherObj == obj)
		return true;
	int dir = -1;
	if (xChange < 0)
		dir = D_LEFT;
	else if (xChange > 0)
		dir = D_RIGHT;
	else if (yChange < 0)
		dir = D_UP;
	else if (yChange > 0)
		dir = D_DOWN;
	if (terrain != NULL && terrain->requestEntry(obj, dir) && (otherObj == NULL || otherObj->requestEntry(obj,dir)))
	{
		return true;
	}
	else
	{
		return false;
	}
}