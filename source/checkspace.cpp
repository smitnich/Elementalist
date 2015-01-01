#include "base.h"
#include "objectDef.h"
#include "terrain.h"
#include "level.h"
extern char solidArray[MAP_SIZE][MAP_SIZE];
extern int posX, posY;
extern int pressureCount;
extern int totalPressureCount;
extern class Level *allLevels[MAX_LEVEL];
Level *getCurrentLevel();
//Checks if the given coordinate is solid
//Recursively calls itself in the case of movable objects until a wall or empty
//space is found
char checkNextSolid(int x, int y)
{
	if (solidArray[posX+x][posY+y] == 0)
		return 0;
	else if (solidArray[posX+x][posY+y] == 2)
		return checkNextSolid(posX+x,posY+y);
	else
		return 1;
}
//Checks the space with the given x or y change relative to the player
char checkSolid(int xChange, int yChange)
{
	int xdir = 0;
	int ydir = 0;
	if (xChange < 0)
		xdir = -1;
	else if (xChange > 0)
		xdir = 1;
	if (yChange < 0)
		ydir = -1;
	else if (yChange > 0)
		ydir = 1;
	if (solidArray[posX+xChange][posY+yChange] == 2)
		return (checkNextSolid(xChange+xdir, yChange+ydir));
	else
		return solidArray[posX+xChange][posY+yChange];
}
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
//Checks the space given a coordinate to start with
//and a direction to go in
char checkSolid(int x, int y, int xChange, int yChange)
{
	int xdir = 0;
	int ydir = 0;
	if (xChange < 0)
		xdir = -1;
	else if (xChange > 0)
		xdir = 1;
	if (yChange < 0)
		ydir = -1;
	else if (yChange > 0)
		ydir = 1;
	if (x < 0 || x > MAP_SIZE || y < 0 || y > MAP_SIZE)
		return 1;
	if (solidArray[x+xChange][y+yChange] == 2)
	{
		return (checkSolid(x,y,xChange+xdir, yChange+ydir));
	}
	if (solidArray[x+xChange][y+yChange] == 3)
	{
		if (pressureCount == 0)
			return 0;
		else
			return 1;
	}
	else
		return solidArray[x+xChange][y+yChange];
}