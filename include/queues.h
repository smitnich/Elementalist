#include <list>
struct SwitchRequest {
	Object *obj;
	Object *switchTo;
};
class Trigger;
class Object;

extern std::list<SwitchRequest> objectSwitchQueue;

void resetSwitchQueue();
Object *addSwitchQueue(Object *in, int switchId);

extern std::list<Trigger*> activateQueue;

void resetActivateQueue();
void addActivateQueue(Trigger *in);
void doActivateQueue();

extern std::list<Object*> creationQueue;

void resetCreationQueue();
void addCreationQueue(Object *in);
void checkCreationQueue();
void creationQueueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY);

struct deleteNode {
	Object *object;
	Terrain *terrain;
};

extern std::list<deleteNode> deleteQueue;

void resetDeleteQueue();
void addDeleteQueue(Object *in);
void addDeleteQueue(Terrain *in);
void doDeleteQueue();

struct MoveRequest
{
	Object *obj;
	int x, y;
	int checkX, checkY;
};

extern std::list<MoveRequest> moveQueue;

void resetMoveQueue();
void queuePlaceAll();
void removeMoveRequest(Object *remove);
void queueDrawAll(SDL_Surface *dest, int moveFractionX, int moveFractionY);
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY);

extern std::list<Object*> objQueue;

void addRenderQueue(Object *obj);
void renderHighSprites(int moveFractionX, int moveFractionY);

extern std::list<SwitchRequest> objectSwitchQueue;

void resetSwitchQueue();
Object *addSwitchQueue(Object *in, int switchId);
void doSwitchQueue();