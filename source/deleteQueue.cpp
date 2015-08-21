#include <list>
#include "level.h"

class Object;

struct deleteNode {
	Object *object;
	Terrain *terrain;
};

std::list<deleteNode> deleteQueue;

void removeMoveRequest(Object *obj);


void resetDeleteQueue()
{
	deleteQueue.clear();
}
void addDeleteQueue(Object *in)
{
	deleteNode tmp = { NULL, NULL };
	tmp.object = in;
	deleteQueue.push_back(tmp);
}
void addDeleteQueue(Terrain *in) {
	deleteNode tmp = { NULL, NULL };
	tmp.terrain = in;
	deleteQueue.push_back(tmp);
}
void doDeleteQueue()
{
	deleteNode tmpNode;
	int size = deleteQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmpNode = deleteQueue.front();
		if (tmpNode.object != NULL) {
			Object *tmp = tmpNode.object;
			removeMoveRequest(tmp);
			getCurrentLevel()->assignObject(tmp->x, tmp->y, NULL);
			delete tmp;
			deleteQueue.pop_front();
		}
		else if (tmpNode.terrain != NULL) {
			delete tmpNode.terrain;
			deleteQueue.pop_front();
		}
	}
}