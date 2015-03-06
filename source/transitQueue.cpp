#include <list>
#include "terrain.h"
std::list<Object*> transitQueue;
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY, bool doDir[4]);

void resetTransitQueue()
{
	transitQueue.clear();
}
void addTransitQueue(Object *in)
{
	transitQueue.push_back(in);
}
void checkTransitQueue()
{
	Object *tmp = NULL;
	int size = transitQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = (Object *)transitQueue.front();
		tmp->objMove();
		transitQueue.pop_front();
		if (tmp->objMoveFraction != 0)
			transitQueue.push_back(tmp);
	}
}
void transitQueueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY, bool *doDir)
{
	for (std::list<Object *>::iterator it = transitQueue.begin(); it != transitQueue.end(); ++it)
	{
		doDraw(*it, moveFractionX, moveFractionY, doDir);
	}
}