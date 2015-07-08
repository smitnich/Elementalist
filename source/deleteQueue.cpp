#include <list>
#include "level.h"

class Object;

std::list<Object*> deleteQueue;

void removeMoveRequest(Object *obj);


void resetDeleteQueue()
{
	deleteQueue.clear();
}
void addDeleteQueue(Object *in)
{
	deleteQueue.push_back(in);
}
void doDeleteQueue()
{
	Object *tmp = NULL;
	int size = deleteQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = (Object *)deleteQueue.front();
		removeMoveRequest(tmp);
		getCurrentLevel()->assignObject(tmp->x, tmp->y, NULL);
		delete tmp;
		deleteQueue.pop_front();
	}
}