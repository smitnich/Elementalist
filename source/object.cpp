#include "object.h"
#include "objectDef.h"
#include "level.h"
class Level* getCurrentLevel();
extern double delta;
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
//Contains the position of the object, as well as whether its solid.
//Used for adding and deleting objects
struct Coordinate
{
	int x;
	int y;
	Object* address;
	int solid;
};
//A buffer of coordinates used for modifying the solid and object arrays

//The basic template for the Objects in the game
		Object::Object()
		{
			level = NULL;
			isPlayer = 0;
			x = 0;
			y = 0;
			objMoveFraction = 0;
			objMoveDir = 0;
			solid = 0;
			pushable = 0;
			moveSpeed = 0;
			tempSpeed = 0;
			frozen = 0;
			this->frozen = 0;
			this->nextObject = NULL;
			ownAddress = this;
			numFrames = 1;
			faceDir = 0;
			lastTicks = 0;
			Level *tmp = getCurrentLevel();
		}
		//Clean up the object and reorganize the linked list
		void Object::deleteMe()
		{
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
		bool Object::tryMove(int x, int y, int dir)
		{
			int checkX = 0;
			int checkY = 0;
			if (dir == D_LEFT)
				checkX = -1;
			else if (dir == D_RIGHT)
				checkX = 1;
			else if (dir == D_DOWN)
				checkY = 1;
			else if (dir == D_UP)
				checkY = -1;
			if (requestMove(x,y,checkX,checkY,this))
			{
				objMoveDir = dir;
				objMoveFraction += tempSpeed*delta;
			}
			return false;
		}
		void Object::doLogic()
		{
		}
		bool Object::requestEntry(Object *other, int dir)
		{
			return false;
		}
		//Gets the sprite to be drawn on the screen
		SDL_Surface* Object::getSprite()
		{
			int newFraction = 0;
			if (objMoveFraction < 48)
				newFraction = floor(objMoveFraction);
			int value = newFraction*numFrames/tileSize;
			if (faceDir != 0 && objMoveFraction == 0)
			{
				if (faceDir == D_RIGHT)
					return spriteew[numFrames];
				else if (faceDir == D_LEFT)
					return spriteew[0];
				else if (faceDir == D_UP)
					return spritens[0];
				else if (faceDir == D_DOWN)
					return spritens[numFrames];
			}
			if (objMoveDir == D_RIGHT || objMoveDir == D_DOWN)
				value += numFrames;
			if (this->objMoveDir == D_LEFT || objMoveDir == D_RIGHT)
				return spriteew[value];
			else if (objMoveDir == D_UP || objMoveDir == D_DOWN)
				return spritens[value];
			else
				return stationary;
		}
		Object* Object::getNext()
		{
			return this->nextObject;
		}
		void Object::setNext(Object* temp)
		{
			this->nextObject = temp;
		}
		void Object::setPrev(Object* temp)
		{
			this->prevObject = temp;
		}
//Objects that stop the player from moving into them
	SolidObject::SolidObject()
	{
		this->isPlayer = 0;
		solid = 1;
		level = getCurrentLevel();
	}
	void SolidObject::doLogic()
	{

	}
//The barrier that opens when all switches are depressed
	Barrier::Barrier(int x2, int y2) : SolidObject()
	{
		this->x = x2;
		this->y = y2;
		this->stationary = barrierTile;
		solidArray[x][y] = this->solid;
		level = getCurrentLevel();
	}
	void Barrier::doLogic()
	{
		if (pressureCount == totalPressureCount)
		{
			this->stationary = NULL;
			this->solid = 0;
			solidArray[x][y] = solid;
		}
		else
		{
			this->stationary = barrierTile;
			this->solid = 1;
			solidArray[x][y] = solid;
		}
	}
	bool Barrier::requestEntry(Object *other, int dir)
	{
		return false;
	}
//Objects that move in some way
	Movable::Movable() : SolidObject()
	{
		objMoveDir = 0;
		objMoveFraction = 0;
		moveSpeed = 0;
		tempSpeed = 0;
		frozen = 0;
		faceDir = 0;
		level = getCurrentLevel();
	}
	void Movable::specialLogic()
	{
		objMove();
	}
	void Movable::objMove()
	{
		int checkX = 0;
		int checkY = 0;
		if (objMoveDir == D_NONE)
			return;
		if (objMoveDir == D_LEFT)
			checkX = -1;
		else if (objMoveDir == D_UP)
			checkY = -1;
		else if (objMoveDir == D_DOWN)
			checkY = 1;
		else if (objMoveDir == D_RIGHT)
			checkX = 1;
		//Make sure that we can move where we're trying to
		if (requestMove(x,y,checkX,checkY,this))
		{
			objMoveFraction+=tempSpeed*delta;
		}
		//Otherwise bounce back
		else
		{
			objMoveFraction-=tempSpeed*delta;
			if (objMoveFraction <= 0)
			{
				objMoveDir = D_NONE;
				objMoveFraction = 0;
			}
		}
		/*If the object has reached the next tile, reset its move variable
		and increment its position variable*/
		if (objMoveFraction >= tileSize)
		{
			Level *lev = getCurrentLevel();
			lev->assignObject(x,y,NULL);
			if (objMoveDir == D_LEFT)
				x-=1;
			else if (objMoveDir == D_RIGHT)
				x+=1;
			else if (objMoveDir == D_UP)
				y-=1;
			else if (objMoveDir == D_DOWN)
				y+=1;
			if (this->frozen == 0)
				objMoveDir = D_NONE;
			objMoveFraction = 0;
			lev->assignObject(x,y,this);
			lev->mapLayer[lev->convertIndex(x,y)]->OnEnter(this);
		}
	}
	void Movable::doLogic()
	{
	}
	void Movable::startMove(int dir)
	{
		objMoveDir = dir;
		tempSpeed = (double) 3*fpsModifier;
	}
//Player created object that moves in one direction and freezes objects on collision
	IceBall::IceBall()
	{
		level = getCurrentLevel();
	}
	IceBall::~IceBall()
	{
		this->deleteMe();
	}
	IceBall::IceBall(int x2, int y2, int moveDir, int moveFraction)
	{
		this->x = x2;
		this->y = y2;
		this->objMoveDir = moveDir;
		this->objMoveFraction = moveFraction;
		this->solid = 0;
		this->stationary = iceBallns[0];
		this->faceDir = 0;
		numFrames = 3;
		for (int i = 0; i < 2*numFrames; i++)
		{
			this->spriteew[i] = iceBallew[i];
			this->spritens[i] = iceBallns[i];
		}
	}
	void IceBall::doLogic()
	{
		objMove();
	}
	void IceBall::objMove()
	{
		int checkX = 0;
		int checkY = 0;
		if (objMoveDir != 0)
		{
			objMoveFraction += movespeed*2;
			if (objMoveFraction >= tileSize)
			{
				if (objMoveDir == D_LEFT)
					x -= 1;
				else if (objMoveDir == D_RIGHT)
					x += 1;
				else if (objMoveDir == D_UP)
					y -= 1;
				else if (objMoveDir == D_DOWN)
					y += 1;
				objMoveFraction -= 48;
			}
			if (solidArray[x][y] == 1)
			{

				delete this;
				return;
			}
		}
	}
	bool IceBall::requestEntry(Object *other, int dir)
	{
		return true;
	}
//Takes in pointers to ovefractionx and y and modifies their values based on direction
void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY, bool *doDir)
{
	switch (moveDir)
	{
	case D_UP:
		*moveFractionY = moveFraction*-1;
		doDir[D_UP-1] = 1;
		break;
	case D_DOWN:
		*moveFractionY = moveFraction;
		doDir[D_DOWN-1] = 1;
		break;
	case D_LEFT:
		*moveFractionX = moveFraction*-1;
		doDir[D_LEFT-1] = 1;
		break;
	case D_RIGHT:
		*moveFractionX = moveFraction;
		doDir[D_RIGHT-1] = 1;
		break;
	default:
		break;
	}
}
//Gets the coordinates for the object and draws it to the screen
void doDraw(Object *drawObject, int moveFractionX, int moveFractionY, bool doDir[4])
{
	SDL_Surface *toDraw = drawObject->getSprite();
	int x = drawObject->getX();
	int y = drawObject->getY();
	int nextX = x+doDir[D_DOWN-1]-doDir[D_UP-1];
	int nextY = y+doDir[D_RIGHT-1]-doDir[D_LEFT-1];
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
	//If the object is within the window
	if ((x+1 > posX-(tilesX)-doDir[D_LEFT-1]) && (x-1 < posX+(tilesX)+doDir[D_RIGHT-1]) && (y+1 > posY-(tilesY) - doDir[D_UP-1]) && (y-1 < posY+(tilesY)+ doDir[D_DOWN-1]))
	{
		int drawX = (x-posX+tilesX)*tileSize+xInitial-moveFractionX+objectMoveFractionx;
		int drawY = (y-posY+tilesY)*tileSize+yInitial-moveFractionY+objectMoveFractiony;
		//Keep rendering window still when moving towards the edges
		if (posX <= 3 || (posX == tilesX && player->objMoveDir != D_RIGHT))
			drawX = x*tileSize+xInitial+objectMoveFractionx;
		else if (posX >= MAP_SIZE-tilesX || (posX == MAP_SIZE-tilesX-1 && player->objMoveDir != D_LEFT))
			drawX = (x-MAP_SIZE+tilesX*2+1)*tileSize+xInitial+objectMoveFractionx;
		if (posY <= 3 || (posY == 4 && player->objMoveDir != D_DOWN))
			drawY = y*tileSize+yInitial+objectMoveFractiony;
		else if (posY >= MAP_SIZE-tilesX || (posY == MAP_SIZE-tilesX-1 && player->objMoveDir != D_UP))
			drawY = (y-MAP_SIZE+tilesX*2+1)*tileSize+yInitial+objectMoveFractiony;
		//Determine which portion of the tile to be rendered when on the edge
		if (drawX <= xInitial && drawX >= xInitial-tileSize)
		{
			drawPortionX = -tileSize+xInitial-drawX;
		}
		else if (drawX >= xInitial+(tileSize*(tilesX*2+1))-tileSize)
		{
			drawPortionX = xInitial+(tileSize*(tilesX*2+1))-drawX;
		}
		if (drawY <= yInitial && drawY >= yInitial-tileSize)
		{
			drawPortionY = -tileSize+yInitial-drawY;
		}
		else if (drawY >= yInitial+(tileSize*(tilesY*2+1))-tileSize)
		{
			drawPortionY = yInitial+(tileSize*(tilesY*2+1))-drawY;
		}
		//if (drawX >= xInitial && drawX < (tilesX*tileSize)+xInitial && drawY >= yInitial && drawY < (tilesY*tileSize)+yInitial)
		apply_surface(drawX,drawY,drawPortionX,drawPortionY,toDraw,screen);
		//Overlay the iceblock graphic if the object is frozen
		if (drawObject->frozen == 1)
		{
			apply_surface(drawX,drawY,drawPortionX,drawPortionY,iceBlock,screen);
		}
	}
}
Object* objectInit(char id, int x, int y)
{
	char buffer[1024];
	Object *newObject;
	switch(id)
	{
	case OBJ_HEAVY_BLOCK:
		newObject = new HeavyCrate(x,y);
		break;
	case OBJ_BLOCK:
		newObject = new Crate(x,y);
		break;
	case OBJ_ICE_BLOCK:
		newObject = new Crate(x,y);
		newObject->frozen = true;
		break;
	case OBJ_PLAYER:
		newObject = new Person(x,y);
		player = newObject;
		break;
	case OBJ_ICE_ELEMENT_E:
		newObject = new IceBall(x,y,D_RIGHT,0);
		break;
	case OBJ_ICE_ELEMENT_N:
		newObject = new IceBall(x,y,D_UP,0);
		break;
	case OBJ_ICE_ELEMENT_S:
		newObject = new IceBall(x,y,D_DOWN,0);
		break;
	case OBJ_ICE_ELEMENT_W:
		newObject = new IceBall(x,y,D_LEFT,0);
		break;
	default:
		sprintf(buffer,"Unable to initialize object %d",id);
		outputLog(buffer);
		return NULL;
		break;
	}
	Level *curLevel = getCurrentLevel();
	curLevel->assignObject(x,y,newObject);
	return newObject;
}
//Creates a new object
Object* objectInit(char id, int x, int y, int moveDir, int moveFraction)
{
	Object *newObject;
	switch(id)
	{
	case '@':
		newObject = new Person(x,y);
		player = newObject;
		break;
	case 'b':
		newObject = new Crate(x,y);
		break;
	case 'B':
		newObject = new HeavyCrate(x,y);
		break;
	case 'i':
		newObject = new IceBall(x,y,moveDir,moveFraction);
		break;
	case 'e':
		newObject = new Barrier(x,y);
		break;
	default:
		return NULL;
		break;
	}
	Level *curLevel = getCurrentLevel();
	curLevel->assignObject(x,y,newObject);
	return newObject;
}
//Clears all objects as well as the linked list and sentinels
void clearObjects()
{
}
//Iterates through the object linked list and draws all of them
void objectDraw()
{
	int moveFractionX = 0;
	int moveFractionY = 0;
	bool doDir[4] = {0,0,0,0};
	calculateMoveFraction(player->objMoveDir,player->objMoveFraction,&moveFractionX,&moveFractionY,doDir);
	Level *curLevel = getCurrentLevel();
	if (curLevel == NULL)
		return;
	for (int x = 0; x < curLevel->width; x++)
	{
		for (int y = 0; y < curLevel->height; y++)
		{
			Object *obj = curLevel->getObject(x,y);
			if (obj != NULL)
			{
				doDraw(obj,moveFractionX, moveFractionY, doDir);
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
		if (tmp != NULL)
			tmp->doLogic();
	}
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
//Checks which object should be placed first (North most = higher, westmost for same y coordinate)
//Returns 1 if one is greater, -1 if two is greater, 0 if they are the same
int compareCoordinates(Object* one, Object* two)
{
	int x1 = one->getX();
	int y1 = one->getY();
	int x2 = two->getX();
	int y2 = two->getY();
	if (y1 > y2)
		return 1;
	else if (y1 < y2)
		return -1;
	else
	{
		if (x1 > x2)
			return 1;
		if (x1 < x2)
			return -1;
	}
	return 0;
}
