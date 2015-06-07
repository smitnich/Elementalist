//Used for rendering sprites that should appear above other sprites
#include "sdlFiles.h"
#include <list>
class Object;

void doDraw(Object *obj, int moveFractionX, int moveFractionY);

std::list<Object*> objQueue;

void addRenderQueue(Object *obj) {
	objQueue.push_front(obj);
}
void renderHighSprites(int moveFractionX, int moveFractionY) {
	while (objQueue.size() > 0) {
		doDraw(objQueue.front(), moveFractionX, moveFractionY);
		objQueue.pop_front();
	}
}