#include <list>
#include "objectDef.h"

#include "level.h"

struct SwitchRequest {
	Object *obj;
	Object *switchTo;
};

Object* objectInit(unsigned int id, int x, int y);

std::list<SwitchRequest> objectSwitchQueue;

void resetSwitchQueue()
{
	objectSwitchQueue.clear();
}
Object *addSwitchQueue(Object *in, int switchId)
{
	SwitchRequest req;
	req.obj = in;
	req.switchTo = objectInit(switchId, in->x, in->y);
	getCurrentLevel()->assignObject(in->x, in->y, in);
	objectSwitchQueue.push_back(req);
	return req.switchTo;
}
void doSwitchQueue()
{
	SwitchRequest req;
	int size = objectSwitchQueue.size();
	for (int i = 0; i < size; i++)
	{
		req = objectSwitchQueue.front();
		delete req.obj;
		getCurrentLevel()->assignObject(req.switchTo->x, req.switchTo->y, req.switchTo);
		objectSwitchQueue.pop_front();
	}
}