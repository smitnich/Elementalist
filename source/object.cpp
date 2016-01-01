#include "object.h"
#include "objectDef.h"
#include "level.h"
#include "tileEnum.h"
#include "defs.h"
#include <climits>
#include <algorithm>
#include "imageMacros.h"

#include "queues.h"
#include "player.h"
#include "sound.h"
#include "terrain.h"

#ifdef GEKKO
#include <cmath>
#endif

extern bool won;
//The map which places Object pointers in their actual game locations, used for collision detection
extern int xInitial, yInitial, tilesX, tilesY;
extern int framesPerSecond;
extern int levelStartCounter;
extern std::string levelName;
extern bool displayName;
extern int conveyorSpeed;
extern int levelChange;
extern std::string startLevelName;
extern int lastInput;
extern int currentLevel;
//The current frame
extern int frame;
//The last frame to recieve input
extern unsigned long lastInputTime;
//Whether or not a player has been placed yet
//Used to determine which one to make active
extern double fpsModifier;
//0 = not solid, 1 = solid 2 = solid & pushable

SDL_Surface *personns[6] = { NULL };
SDL_Surface *personew[6] = { NULL };
SDL_Surface *deadPerson = NULL;
SDL_Surface *spr_ash = NULL;

extern double delta;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Object *objectList[MAX_OBJECTS] = { NULL };
bool playerPlaced = 0;

//The distance away from the player at which the sound is fully
//in the left or right direction
#define SOUND_OFFSET_LENGTH 10

//The basic template for the Objects in the game
Object::Object()
{
	isPlayer = false;
	numFrames = 1;
	isMagnetic = false;
	prevMove = D_NONE;
	timeToLive = nan("");
}
Object::Object(int x2, int y2)
{
	memset(this, 0, sizeof(*this));
	isPlayer = false;
	x = x2;
	y = y2;
	numFrames = 1;
	isMagnetic = false;
	hovering = false;
	prevMove = D_NONE;
	timeToLive = nan("");
	dead = false;
	visible = true;
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
	visible = true;
}
void Object::toAsh()
{
	die();
	Object *ash = addSwitchQueue(this, OBJ_ASH);
	if (isPlayer)
		player = ash;
}
//Clean up the object
void Object::die()
{
	if (!dead)
		addDeleteQueue(this);
	dead = true;
}
void Object::electrocute()
{
	if (!frozen)
	{
		toAsh();
		playSound(snd_zap);
	}
}
Object::~Object()
{
	Level *curLevel = getCurrentLevel();
	if (curLevel != NULL)
	{
		if (curLevel->getObject(x,y) == this)
			curLevel->assignObject(x, y, NULL);
	}
}
void Object::updateTime() {
	lifeTime += delta;
	timeToLive -= delta;
	if (timeToLive < 0)
		die();
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
//If this function is called, the object will first attempt to move
//in the same direction as before; failing that it will rotate left
//until it has tried all directions
void Object::preferLeftTurn() {
	int dir = prevMove;
	for (int i = 0; i < 4; i++) {
		if (dir > 4)
			dir = 1;
		if (startMove(dir, 1)) {
			return;
		}
		else if (i == 0)
		{
			int moveFractionX = 0;
			int moveFractionY = 0;
			calculateMoveFraction(dir, 1, &moveFractionX, &moveFractionY);
			Object *collision = getCurrentLevel()->getObject(x + moveFractionX, y + moveFractionY);
			if (collision != NULL) {
				this->onCollision(collision, dir);
				collision->onCollision(this, collision->objMoveDir);
			}
		}
		dir++;
	}
}
//If this function is called, the object will first attempt to move
//in the same direction as before; failing that it will rotate right
//until it has tried all directions
void Object::preferRightTurn() {
	int dir = prevMove;
	for (int i = 0; i < 4; i++) {
		if (dir <= 0)
			dir = 4;
		if (startMove(dir, 1)) {
			return;
		}
		else if (i == 0)
		{
			int moveFractionX = 0;
			int moveFractionY = 0;
			calculateMoveFraction(dir, 1, &moveFractionX, &moveFractionY);
			Object *collision = getCurrentLevel()->getObject(x + moveFractionX, y + moveFractionY);
			if (collision != NULL)
				this->onCollision(collision, dir);
		}
		dir--;
	}
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
	if (objMoveFraction == 0.0) {
		prevMove = objMoveDir;
	}
	if (objMoveDir == D_LEFT)
		checkX = -1;
	else if (objMoveDir == D_UP)
		checkY = -1;
	else if (objMoveDir == D_DOWN)
		checkY = 1;
	else if (objMoveDir == D_RIGHT)
		checkX = 1;
	Object *other = getCurrentLevel()->getObject(x + checkX, y + checkY);
	if (other != NULL && checkCollision(this,other)) {
		other->onCollision(this, objMoveDir);
		onCollision(other, objMoveDir);
	}
	//Make sure that we can move where we're trying to
	if (getCurrentLevel()->getTerrain(x, y)->requestExit(this, objMoveDir) && requestMove(x, y, checkX, checkY, this))
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
		if (this->slides() == false)
			objMoveDir = D_NONE;
		objMoveFraction = 0;
		addMoveRequest(this, x, y, checkX, checkY);
		return true;
	}
	return false;
}
void Object::addConnection(Object *in, int _index)
{
	connection tmp = { NULL, NULL, 0 };
	tmp.obj = in;
	tmp.index = _index;
	connections.push_back(tmp);
	in->totalConnections++;
}
void Object::addConnection(Terrain *in, int _index) {
	connection tmp = { NULL, NULL, 0 };
	tmp.terrain = in;
	tmp.index = _index;
	connections.push_back(tmp);
	in->totalConnections++;
}
void Object::burn()
{
	playSound(snd_burn);
	toAsh();
}
void Object::freeze()
{
	frozen = true;
	objMoveDir = D_NONE;
	objMoveFraction = 0.0f;
	playSound(snd_freeze);
}
void Object::heat(Object *heatObj)
{
	if (frozen)
		playSound(snd_melt);
	frozen = false;
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
	if (!drawObject->visible)
		return;
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
Object* objectInit(unsigned int id, int x, int y, bool addToLevel)
{
	Object *newObject = objectList[id-1000]->createInstance(x,y);
	if (newObject == NULL)
		exit(0);
	//If this is the player:
	if (id == 1004)
		player = newObject;
	newObject->id = id;
	Level *curLevel = getCurrentLevel();
	if (addToLevel)
		curLevel->assignObject(x, y, newObject);
	return newObject;
}
Object *objectInit(unsigned int id, int idx, bool addToLevel) {
	return objectInit(id, idx % TILE_SIZE, idx / TILE_SIZE, addToLevel);
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
			if (tmp->objMoveFraction < TILE_SIZE/2)
				curLevel->getTerrain(tmp->x, tmp->y)->whileIn(tmp);
			//The Object may have been deleted during the while in call,
			//so reload it here
			tmp = curLevel->objectLayer.at(i);
			if (tmp != NULL)
				tmp->update();
		}
	}
	queuePlaceAll();
	checkCreationQueue();
	doSwitchQueue();
	doDeleteQueue();
	doLogicTerrain();
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
int oppositeDir(int dir) {
	switch (dir) {
	case D_LEFT:
		return D_RIGHT;
	case D_RIGHT:
		return D_LEFT;
	case D_UP:
		return D_DOWN;
	case D_DOWN:
		return D_UP;
	default:
		return D_NONE;
	}
}
//Determines whether or not two objects will collide within the square that
//first is moving towards
//First- the object that is moving towards the second object
bool checkCollision(Object *first, Object *second) {
	if (second->objMoveDir == D_NONE)
		return true;
	if (first->objMoveDir == oppositeDir(second->objMoveDir))
		return true;
	float secondMoveTime = ((float) TILE_SIZE - second->objMoveFraction) / ((float) second->tempSpeed);
	float firstMoveTime = ((float) TILE_SIZE - first->objMoveFraction) / ((float) first->tempSpeed);
	if (firstMoveTime < secondMoveTime)
		return true;
	else
		return false;
}
void Object::playSound(Mix_Chunk *sound) {
	const int maxPanning = 254;
	const int defaultPanning = 127;
	int channel = Mix_PlayChannel(-1, sound, 0);
	int offset = player->x - x;
	if (offset < -SOUND_OFFSET_LENGTH)
		offset = -SOUND_OFFSET_LENGTH;
	else if (offset > SOUND_OFFSET_LENGTH)
		offset = SOUND_OFFSET_LENGTH;
	int panning = defaultPanning - (offset * (maxPanning-defaultPanning)/SOUND_OFFSET_LENGTH);
	setPanning(channel, panning);
}