#include "objectDef.h"
#include "base.h"
#include "sprites.h"
#include "input_def.h"
#include "level.h"
#include "debugText.h"
extern int posX, posY;
extern int pressureCount;
extern int lastMoveDir;
extern Uint32 lastInputTime;
extern int frame;
extern bool playerPlaced;
extern bool displayName;
extern bool won;
extern int lastInput;
extern int currentLevelNum;
extern int levelChange;
extern string startLevelName;
void moveLine(int, int, int);
void startMove(int dir);
extern double movespeed;
extern int framesPerSecond;
extern int levelStartCounter;
bool playerDead = false;
int getInput();
void cleanup();
void changeTextToDead();
class Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
Uint32 getTicks();
extern double delta;
Object* objectInit(unsigned int id, int x, int y, int moveDir, int moveFraction);
//The player
Person::Person(const Person &other, int _x, int _y)
{
	active = false;
	level = other.level;
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
	for (int i = 0; i < 6; i++)
	{
		spriteew[i] = other.spriteew[i];
		spritens[i] = other.spritens[i];
	}
}
Object* Person::clone(int _x, int _y)
{
	return new Person(*this,_x,_y);
}
	Person::Person(int x2, int y2)
	{
		level = getCurrentLevel();
		stationary = personns[3];
		numFrames = 3;
		for (int i = 0; i < numFrames * 2; i++)
		{
			this->spriteew[i] = personew[i];
			this->spritens[i] = personns[i];
		}
		isPlayer = true;
		x = x2;
		y = y2;
		objMoveDir = 0;
		objMoveFraction = 0;
		solid = 0;
		faceDir = 0;
		prevMove = D_NONE;
		hovering = false;
		if (playerPlaced == false)
		{
			active = 1;
			playerPlaced = true;
		}
		else
		{
			this->active = 0;
		}
		lastMoveDir = D_NONE;
	}
	void Person::makeElement(bool doSecond)
	{
		return;
	}
	void Person::die()
	{
		faceDir = objMoveDir = D_NONE;
		playerDead = true;
		stationary = deadPerson;
		changeTextToDead();
	}
	bool Person::requestEntry(Object *other, int dir)
	{
		int checkX = 0;
		int checkY = 0;
		switch (dir)
		{
		case D_UP:
			checkY = -1;
			break;
		case D_DOWN:
			checkY = 1;
			break;
		case D_LEFT:
			checkX = -1;
			break;
		case D_RIGHT:
			checkX = 1;
			break;
		//If we're not approaching in any direction, then we are at level initilization and wish
		//to allow the object to enter
		case -1:
			return true;
		}
		if (!requestMove(x, y, checkX, checkY, level->getObject(x + checkX, y + checkY)))
		{
			return false;
		}
		startMove(dir);
		return true;
	}
	//Moves, gets input if needed and checks for forces on the player eg conveyor belts
	void Person::doLogic()
	{
		if (active == false && objMoveDir == D_NONE)
			return;
		objMove();
		if (displayName == true)
		{
			int input = getInput();
			if (input != INPUT_NONE)
			{
				if (getTicks() - lastInputTime > 1000)
				{
					displayName = false;
				}
			}
		}
		else if (won == true)
		{
			if (currentLevelNum >= MAX_LEVEL - 1)
				exit(0);
			levelChange = currentLevelNum+1;
			startLevelName.assign("");
		}
		else if (playerDead == true)
		{
			int input = getInput();
			if (input != INPUT_NONE)
			{
				if (getTicks() - lastInputTime > 1000 || input != lastInput)
				{
					playerDead = false;
					levelChange = currentLevelNum;
				}
			}

		}
		else
		{
			int input = getInput();
			if (input != INPUT_NONE)
				lastInputTime = getTicks();
			switch (input){
			case BUTTON_MENU:
				cleanup();
				break;
			case BUTTON_LEVEL_NEXT:
				if (currentLevelNum < MAX_LEVEL-1 && lastInput != BUTTON_LEVEL_NEXT)
				{
					lastInput = BUTTON_LEVEL_NEXT;
					levelChange = currentLevelNum+1;
					startLevelName.assign("");
					return;
				}
				lastInput = BUTTON_LEVEL_NEXT;
				break;
			case BUTTON_LEVEL_PREV:
				if (currentLevelNum > 1 && lastInput != BUTTON_LEVEL_PREV)
				{
					lastInput = BUTTON_LEVEL_PREV;
					levelChange = currentLevelNum-1;
					startLevelName.assign("");
					return;
				}
				lastInput = BUTTON_LEVEL_PREV;
				break;
			case BUTTON_1:
				if (lastInput != BUTTON_1)
				{
					debugOn = !debugOn;
					lastInput = BUTTON_1;
				}
				break;
			case BUTTON_2:
				if (lastInput != BUTTON_2)
				{
					lastInput = BUTTON_2;
				}
				break;
			case B_RIGHT:
				if (x < MAP_SIZE && objMoveFraction == 0 && !frozen)
				{
					startMove(D_RIGHT);
					lastMoveDir = D_RIGHT;
					faceDir = D_RIGHT;
				}
				break;
			case B_LEFT:
				if (x > 0 && objMoveFraction == 0 && !frozen)
				{
					startMove(D_LEFT);
					lastMoveDir = D_LEFT;
					faceDir = D_LEFT;
				}
				lastInput = B_LEFT;
				break;
			case B_DOWN:
				if (y < MAP_SIZE && objMoveFraction == 0 && !frozen)
				{
					startMove(D_DOWN);
					faceDir = D_DOWN;
					lastMoveDir = D_DOWN;
				}
				lastInput = B_DOWN;
				break;
			case B_UP:
				if (y > 0 && objMoveFraction == 0 && !frozen)
				{
					startMove(D_UP);
					faceDir = D_UP;
					lastMoveDir = D_UP;
				}
				lastInput = B_UP;
				break;
			default:
				//lastInput = B_NONE;
				break;
			}
		}
	}