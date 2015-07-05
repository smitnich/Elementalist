#include "object.h"
#include "objectDef.h"
#include "level.h"
#include "tileEnum.h"
#include "defs.h"
#include "moveRequest.h"
#include <climits>

SDL_Surface *personns[6] = { NULL };
SDL_Surface *personew[6] = { NULL };
SDL_Surface *deadPerson = NULL;
class Level* getCurrentLevel();
extern double delta;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Level *getCurrentLevel();
void checkCreationQueue();
void writeDebugText(char* in);
Object *objectList[MAX_OBJECTS] = { NULL };
extern std::list<Object*> creationQueue;
extern std::list<MoveRequest> moveQueue;

//The basic template for the Objects in the game
Object::Object()
{
	isPlayer = false;
	x = 0;
	y = 0;
	objMoveFraction = 0;
	objMoveDir = 0;
	solid = 0;
	moveSpeed = 0;
	tempSpeed = 0;
	frozen = 0;
	numFrames = 1;
	faceDir = 0;
	isMagnetic = false;
}
Object::Object(int x2, int y2)
{
	isPlayer = false;
	x = x2;
	y = y2;
	objMoveFraction = 0;
	objMoveDir = 0;
	solid = 0;
	moveSpeed = 0;
	tempSpeed = 0;
	frozen = 0;
	numFrames = 1;
	faceDir = 0;
	isMagnetic = false;
	hovering = false;
	currentMovePriority = 0;
}
Object::Object(Object &other, int _x, int _y)
{
	isPlayer = other.isPlayer;
	x = _x;
	y = _y;
	objMoveFraction = 0;
	objMoveDir = 0;
	solid = other.solid;
	moveSpeed = other.moveSpeed;
	tempSpeed = 0;
	frozen = other.frozen;
	numFrames = other.numFrames;
	faceDir = 0;
}
//Clean up the object
void Object::die()
{
	delete this;
}
Object::~Object()
{
	getCurrentLevel()->assignObject(x, y, NULL);
}
int Object::getMoveDir()
{
	return objMoveDir;
}
double Object::getMoveFraction()
{
	return objMoveFraction;
}
int Object::getX()
{
	return x;
}
int Object::getY()
{
	return y;
}
void Object::doLogic()
{
}
bool Object::isMovableBlock()
{
	return false;
}
bool Object::startMove(int dir, int priority)
{
	if (priority < currentMovePriority)
		return false;
	int xChange = 0;
	int yChange = 0;
	switch (dir) {
	case D_UP:
		yChange = -1;
		break;
	case D_DOWN:
		yChange = 1;
		break;
	case D_LEFT:
		xChange = -1;
		break;
	case D_RIGHT:
		xChange = 1;
		break;
	}
	if (dir == D_NONE || !requestMove(x,y,xChange,yChange,this))
		return false;
	objMoveDir = dir;
	tempSpeed = (double)3 * fpsModifier;
	return true;
}
bool Object::requestEntry(Object *other, int dir)
{
	return false;
}
//Objects that move in some way
bool Object::objMove()
{
	int checkX = 0;
	int checkY = 0;
	if (objMoveDir == D_NONE)
		return false;
	if (objMoveDir == D_LEFT)
		checkX = -1;
	else if (objMoveDir == D_UP)
		checkY = -1;
	else if (objMoveDir == D_DOWN)
		checkY = 1;
	else if (objMoveDir == D_RIGHT)
		checkX = 1;
	Object *other = getCurrentLevel()->getObject(x + checkX, y + checkY);
	if (other != NULL) {
		other->onEnterStart(this, objMoveDir);
	}
	//Make sure that we can move where we're trying to
	if (getCurrentLevel()->getTerrain(x,y)->requestExit(this,objMoveDir) && requestMove(x, y, checkX, checkY, this))
	{
		objMoveFraction += tempSpeed*delta;
	}
	//Otherwise bounce back
	else
	{
		objMoveFraction -= tempSpeed*delta;
		if (objMoveFraction <= 0)
		{
			objMoveDir = D_NONE;
			objMoveFraction = 0;
		}
	}
	/*If the object has reached the next tile, reset its move variable
	and increment its position variable*/
	if (objMoveFraction >= TILE_SIZE)
	{
		Level *lev = getCurrentLevel();
		if (lev->getObject(x,y) == this)
			lev->assignObject(x, y, NULL);
		prevMove = objMoveDir;
		if (objMoveDir == D_LEFT)
			checkX = -1;
		else if (objMoveDir == D_RIGHT)
			checkX = 1;
		else if (objMoveDir == D_UP)
			checkY = -1;
		else if (objMoveDir == D_DOWN)
			checkY = 1;
		if (this->frozen == false)
			objMoveDir = D_NONE;
		objMoveFraction = 0;
		addMoveRequest(this, x, y, checkX, checkY);
		return true;
	}
	return false;
}
//Takes in pointers to movefractionx and y and modifies their values based on direction
void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY)
{
	switch (moveDir)
	{
	case D_UP:
		*moveFractionY = moveFraction*-1;
		break;
	case D_DOWN:
		*moveFractionY = moveFraction;
		break;
	case D_LEFT:
		*moveFractionX = moveFraction*-1;
		break;
	case D_RIGHT:
		*moveFractionX = moveFraction;
		break;
	default:
		break;
	}
}
//Gets the coordinates for the object and draws it to the screen
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY)
{
	int posX = 0;
	int posY = 0;
	if (player != NULL)
	{
		posX = player->x;
		posY = player->y;
	}
	SDL_Surface *toDraw = drawObject->getSprite();
	int x = drawObject->getX();
	int y = drawObject->getY();
	int ObjectMovedir = drawObject->getMoveDir();
	int objectMoveFractionx = 0;
	int objectMoveFractiony = 0;
	if (ObjectMovedir == D_LEFT)
		objectMoveFractionx = (drawObject->getMoveFraction())*-1;
	else if (ObjectMovedir == D_RIGHT)
		objectMoveFractionx = drawObject->getMoveFraction();
	else if (ObjectMovedir == D_UP)
		objectMoveFractiony = (drawObject->getMoveFraction())*-1;
	else if (ObjectMovedir == D_DOWN)
		objectMoveFractiony = drawObject->getMoveFraction();
	int drawPortionX = 0;
	int drawPortionY = 0;
	int drawX = (x - posX + tilesX)*TILE_SIZE + xInitial - moveFractionX + objectMoveFractionx;
	int drawY = (y - posY + tilesY)*TILE_SIZE + yInitial - moveFractionY + objectMoveFractiony;
	//Keep rendering window still when moving towards the edges
	if (posX <= 3 || (posX == tilesX && player->objMoveDir != D_RIGHT))
		drawX = x*TILE_SIZE + xInitial + objectMoveFractionx;
	else if (posX >= MAP_SIZE - tilesX || (posX == MAP_SIZE - tilesX - 1 && player->objMoveDir != D_LEFT))
		drawX = (x - MAP_SIZE + tilesX * 2 + 1)*TILE_SIZE + xInitial + objectMoveFractionx;
	if (posY <= 3 || (posY == 4 && player->objMoveDir != D_DOWN))
		drawY = y*TILE_SIZE + yInitial + objectMoveFractiony;
	else if (posY >= MAP_SIZE - tilesX || (posY == MAP_SIZE - tilesX - 1 && player->objMoveDir != D_UP))
		drawY = (y - MAP_SIZE + tilesX * 2 + 1)*TILE_SIZE + yInitial + objectMoveFractiony;
	//Determine which portion of the tile to be rendered when on the edge
	if (drawX <= xInitial && drawX >= xInitial - TILE_SIZE)
	{
		drawPortionX = -TILE_SIZE + xInitial - drawX;
	}
	else if (drawX >= xInitial + (TILE_SIZE*(tilesX * 2 + 1)) - TILE_SIZE)
	{
		drawPortionX = xInitial + (TILE_SIZE*(tilesX * 2 + 1)) - drawX;
	}
	if (drawY <= yInitial && drawY >= yInitial - TILE_SIZE)
	{
		drawPortionY = -TILE_SIZE + yInitial - drawY;
	}
	else if (drawY >= yInitial + (TILE_SIZE*(tilesY * 2 + 1)) - TILE_SIZE)
	{
		drawPortionY = yInitial + (TILE_SIZE*(tilesY * 2 + 1)) - drawY;
	}
	//if (drawX >= xInitial && drawX < (tilesX*TILE_SIZE)+xInitial && drawY >= yInitial && drawY < (tilesY*TILE_SIZE)+yInitial)
	apply_surface(drawX, drawY, drawPortionX, drawPortionY, toDraw, screen);
	//Overlay the iceblock graphic if the object is frozen
	if (drawObject->frozen == 1)
	{
		apply_surface(drawX, drawY, drawPortionX, drawPortionY, iceBlock, screen);
	}
}
void Object::draw(int moveFractionX, int moveFractionY) {
	doDraw(this, moveFractionX, moveFractionY);
}
Object* objectInit(unsigned int id, int x, int y)
{
	Object *newObject = objectList[id-1000]->createInstance(x,y);
	if (newObject == NULL)
		exit(0);
	//If this is the player:
	if (id == 1004)
		player = newObject;
	newObject->id = id;
	Level *curLevel = getCurrentLevel();
	curLevel->assignObject(x, y, newObject);
	return newObject;
}
void clearObjects()
{
	Level *curLevel = getCurrentLevel();
	if (curLevel == NULL)
		return;
	for (int i = 0; i < curLevel->height*curLevel->width; i++)
	{
		Object *tmp = curLevel->objectLayer.at(i);
		if (tmp != NULL)
			delete(tmp);
	}
}
void objectDraw()
{
	int moveFractionX = 0;
	int moveFractionY = 0;
	calculateMoveFraction(player->objMoveDir, player->objMoveFraction, &moveFractionX, &moveFractionY);
	Level *curLevel = getCurrentLevel();
	if (curLevel == NULL)
		return;
	for (int x = 0; x < curLevel->width; x++)
	{
		for (int y = 0; y < curLevel->height; y++)
		{
			Object *obj = curLevel->getObject(x, y);
			if (obj != NULL)
			{
				doDraw(obj, moveFractionX, moveFractionY);
			}
		}
	}
}
//Iterates through the objects and has each perform its logic, then calls doQueue
void objectLogic()
{
	Level *curLevel = getCurrentLevel();
	for (int i = 0; i < curLevel->height*curLevel->width; i++)
	{
		Object *tmp = curLevel->objectLayer.at(i);
		if (tmp != NULL) {
			curLevel->getTerrain(tmp->x, tmp->y)->whileIn(tmp);
			//The Object may have been deleted during the while in call,
			//so reload it here
			tmp = curLevel->objectLayer.at(i);
			if (tmp != NULL)
				tmp->doLogic();
		}
	}
	queuePlaceAll();
	checkCreationQueue();
}
void doPlayer()
{
	if (player != NULL)
		player->doLogic();
}
double getPlayerMoveFraction()
{
	return player->getMoveFraction();
}
int getPlayerMoveDir()
{
	return player->getMoveDir();
}
//Attempt to find the next closest player to switch to
bool switchPlayerFocus() {
	Level *curLevel = getCurrentLevel();
	Object *closestPlayer = NULL;
	std::list<Object *> allPlayers;
	int x = player->x;
	int y = player->y;
	unsigned int bestDist = UINT_MAX;
	//Find all available players
	for (int i = 0; i < curLevel->height*curLevel->width; i++)
	{
		Object *tmp = curLevel->objectLayer.at(i);
		if (tmp != NULL && tmp->isPlayer && tmp != player)
			allPlayers.push_front(tmp);
	}
	for (std::list<Object *>::iterator it = creationQueue.begin(); it != creationQueue.end(); ++it)
	{
		Object *tmp = *it;
		if (tmp != NULL && tmp->isPlayer && tmp != player) {
			allPlayers.push_front(tmp);
		}
	}
	for (std::list<MoveRequest>::iterator it = moveQueue.begin(); it != moveQueue.end(); ++it)
	{
		Object *tmp = it->obj;
		if (tmp != NULL && tmp->isPlayer && tmp != player) {
			allPlayers.push_front(tmp);
		}
	}
	if (allPlayers.size() == 0)
		return false;
	//Find the player closest to the previous player
	while (allPlayers.size() > 0) {
		Object *tmpPlayer = allPlayers.front();
		unsigned int dist = (x - tmpPlayer->x)*(x - tmpPlayer->x) + (y - tmpPlayer->y)*(y - tmpPlayer->y);
		if (closestPlayer == NULL || dist < bestDist) {
			closestPlayer = tmpPlayer;
			bestDist = dist;
		}
		allPlayers.pop_front();
	}
	player = closestPlayer;
	return true;
}