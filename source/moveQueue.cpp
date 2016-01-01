#include <list>
#include "objectDef.h"
#include "level.h"
#include "queues.h"

Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY);
void writeDebugText(char *in);

std::list<MoveRequest> moveQueue;

void resetMoveQueue()
{
	moveQueue.clear();
}
void queuePlaceAll()
{
	MoveRequest tmp;
	Level *level = getCurrentLevel();
	int size = moveQueue.size();
	int x = 0;
	int y = 0; 
	for (int i = 0; i < size; i++)
	{
		tmp = moveQueue.front();
		moveQueue.pop_front();
		if (tmp.obj == NULL)
			continue;
		x = tmp.x+tmp.checkX;
		y = tmp.y+tmp.checkY;
		Object *blockingObject = level->getObject(x, y);
		tmp.obj->x = x;
		tmp.obj->y = y;
		if (blockingObject == NULL)
		{
			level->assignObject(x, y, tmp.obj);
			Terrain *tmpTerrain = level->getTerrain(x, y);
			tmpTerrain->onEnter(tmp.obj);
		}
		else if (blockingObject->allowEntry())
		{
			blockingObject->onEnter(tmp.obj, tmp.checkX, tmp.checkY);
		}
		else
		{
			moveQueue.push_back(tmp);
		}
	}
}
void removeMoveRequest(Object *remove) {
	std::list<MoveRequest>::iterator it;
	for (it = moveQueue.begin(); it != moveQueue.end(); ++it) {
		if (it->obj == remove) {
			it->obj = NULL;
		}
	}
}
void queueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY)
{
	MoveRequest tmp;
	int size = moveQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = moveQueue.front();
		moveQueue.pop_front();
		if (tmp.obj != NULL) {
			doDraw(tmp.obj, moveFractionX, moveFractionY);
			moveQueue.push_back(tmp);
		}
	}
}
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY)
{
	MoveRequest tmpReq = MoveRequest();
	tmpReq.obj = obj;
	tmpReq.x = x;
	tmpReq.y = y;
	getCurrentLevel()->getTerrain(x, y)->onExit(tmpReq.obj);
	tmpReq.checkX = checkX;
	tmpReq.checkY = checkY;
	tmpReq.obj->x = x+checkX;
	tmpReq.obj->y = y+checkY;
	moveQueue.push_back(tmpReq);
}