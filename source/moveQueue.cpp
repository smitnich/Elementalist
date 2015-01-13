#include "base.h"
#include <list>
#include "objectDef.h"
#include "level.h"
Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
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
	for (int i = 0; i < size; i++)
	{
		tmp = moveQueue.front();
		x = tmp->x+tmp->checkX;
		y = tmp->y+tmp->checkY;
		moveQueue.pop_front();
		if (level->getObject(x,y) == NULL)
		{
			tmp->obj->x = x;
			tmp->obj->y = y;
			level->assignObject(x,y,tmp->obj);
		}
		else
		{
			moveQueue.push_back(tmp);
		}
	}
}
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY)
{
	MoveRequest *tmpReq = new MoveRequest();
	tmpReq->obj = obj;
	tmpReq->x = x;
	tmpReq->y = y;
	tmpReq->checkX = checkX;
	tmpReq->checkY = checkY;
	moveQueue.push_back(tmpReq);
}