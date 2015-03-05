#include <list>
#include "terrain.h"
#include "level.h"
std::list<Object*> assignQueue;

void resetAssignQueue()
{
	assignQueue.clear();
}
void addAssignQueue(Object *in)
{
	assignQueue.push_back(in);
}
void doAssignQueue()
{
	Object *tmp = NULL;
	int size = assignQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = (Object *)assignQueue.front();
		getCurrentLevel()->assignObject(tmp->x, tmp->y, tmp);
		assignQueue.pop_front();
	}
}