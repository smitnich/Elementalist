#include <list>
#include "terrain.h"
//Used when a new object is created using the duplicator: since two objects can't exist
//in the same square, we need to store newly created objects in the queue before they
//move to the next square
std::list<Object*> creationQueue;
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY, bool doDir[4]);

void resetCreationQueue()
{
	creationQueue.clear();
}
void addCreationQueue(Object *in)
{
	creationQueue.push_back(in);
}
void checkCreationQueue()
{
	Object *tmp = NULL;
	int size = creationQueue.size();
	for (int i = 0; i < size; i++)
	{
		tmp = (Object *)creationQueue.front();
		bool moveCompleted = tmp->objMove();
		creationQueue.pop_front();
		if (!moveCompleted)
			creationQueue.push_back(tmp);
	}
}
void creationQueueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY, bool *doDir)
{
	for (std::list<Object *>::iterator it = creationQueue.begin(); it != creationQueue.end(); ++it)
	{
		doDraw(*it, moveFractionX, moveFractionY, doDir);
	}
}