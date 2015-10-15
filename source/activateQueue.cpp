#include <list>
#include "terrain.h"
#include "queues.h"

std::list<Trigger*> activateQueue;

void resetActivateQueue()
{
	activateQueue.clear();
}
void addActivateQueue(Trigger *in)
{
	activateQueue.push_back(in);
}
void doActivateQueue()
{
	Trigger *tmp = NULL;
	int size = activateQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = (Trigger *)activateQueue.front();
		int length = tmp->connections.size();
		for (int i = 0; i < length; i++)
			tmp->connections.at(i).terrain->activate();
		activateQueue.pop_front();
	}
}