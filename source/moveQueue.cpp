#include "base.h"
#include <list>
#include "objectDef.h"
#include "level.h"
Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY, bool doDir[4]);
void writeDebugText(char *in);
struct MoveRequest
{
	Object *obj;
	int x,y;
	int checkX,checkY;
};
std::list<MoveRequest*> moveQueue;

void resetMoveQueue()
{
	moveQueue.clear();
}
void queuePlaceAll()
{
	MoveRequest *tmp;
	Level *level = getCurrentLevel();
	int size = moveQueue.size();
	int x = 0;
	int y = 0; 
	int tmpQueue = D_NONE;
	for (int i = 0; i < size; i++)
	{
		tmp = moveQueue.front();
		x = tmp->x+tmp->checkX;
		y = tmp->y+tmp->checkY;
		moveQueue.pop_front();
		if (level->getObject(x,y) == NULL)
		{
			tmpQueue = tmp->obj->queuedMove;
			level->assignObject(x, y, tmp->obj);
			level->getTerrain(x, y)->onEnter(tmp->obj);
			delete tmp;
		}
		else
		{
			moveQueue.push_back(tmp);
		}
	}
}
void queueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY,bool *doDir)
{
	MoveRequest *tmp;
	int size = moveQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = moveQueue.front();
		moveQueue.pop_front();
		doDraw(tmp->obj,moveFractionX,moveFractionY,doDir);
		moveQueue.push_back(tmp);
	}
}
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY)
{
	MoveRequest *tmpReq = new MoveRequest();
	tmpReq->obj = obj;
	tmpReq->x = x;
	tmpReq->y = y;
	getCurrentLevel()->getTerrain(x, y)->onExit(tmpReq->obj);
	tmpReq->checkX = checkX;
	tmpReq->checkY = checkY;
	tmpReq->obj->x = x+checkX;
	tmpReq->obj->y = y+checkY;
	moveQueue.push_back(tmpReq);
}