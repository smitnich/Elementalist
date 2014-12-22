#include "objectDef.h"
#include "base.h"
#include "sprites.h"
#include "input_def.h"
extern int posX, posY;
extern int pressureCount;
extern int lastMoveDir;
extern int lastInputFrame;
extern int frame;
extern bool playerPlaced;
extern bool displayName;
extern bool won;
extern int lastInput;
extern int currentLevelNum;
extern int levelChange;
//extern char solidArray[MAP_SIZE][MAP_SIZE];
extern string startLevelName;
void moveLine(int, int, int);
void startMove(int dir);
extern double movespeed;
extern int framesPerSecond;
extern int levelStartCounter;
int getInput();
void cleanup();
class Level* getCurrentLevel();
void outputLog(char[]);
void outputLog(int);
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
char checkSolid(int x,int y,int xOff, int yOff);
extern double delta;
Object* objectInit(char id, int x, int y, int moveDir, int moveFraction);
//The player
	Person::Person(int x2, int y2)
	{
		level = getCurrentLevel();
		this->stationary = personns[3];
		this->numFrames = 3;
		for (int i = 0; i < numFrames*2; i++)
		{
			this->spriteew[i] = personew[i];
			this->spritens[i] = personns[i];
		}
		this->element = 2;
		this->isPlayer = 1;
		this->x = x2;
		this->y = y2;
		posX = x2;
		posY = y2;
		this->objMoveDir = 0;
		this->objMoveFraction = 0;
		this->solid = 0;
		this->faceDir = 0;
		if (playerPlaced == 0)
		{
			active = 1;
			playerPlaced = 1;
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
	bool Person::requestEntry(Object *other, int dir)
	{
		return true;
	}
	//Moves, gets input if needed and checks for forces on the player eg conveyor belts
	void Person::doLogic()
	{
		if (active == 0 && objMoveDir == D_NONE)
			return;
		objMove();
		if (displayName == true)
		{
			int input = getInput();
			if (input != INPUT_NONE && input != lastInput)
			{
				if (won == true)
				{
					if (currentLevelNum >= MAX_LEVEL-1)
						exit(0);
					else
						currentLevelNum++;
					levelChange = currentLevelNum;
					startLevelName.assign("");
				}
				if (levelStartCounter == 0)
					displayName = 0;
			}
		}
		else
		{
			int input = getInput();
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
					currentLevelNum--;
					levelChange = currentLevelNum-1;
					startLevelName.assign("");
					return;
				}
				lastInput = BUTTON_LEVEL_PREV;
				break;
			case BUTTON_1:
				if (lastInput != BUTTON_1)
				{
					makeElement(false);
					lastInput = BUTTON_1;
				}
				break;
			case BUTTON_2:
				if (lastInput != BUTTON_2)
				{
					makeElement(true);
					lastInput = BUTTON_2;
				}
			case B_RIGHT:
				if (x < MAP_SIZE && objMoveFraction == 0)
				{
					startMove(D_RIGHT);
					lastMoveDir = D_RIGHT;
					faceDir = D_RIGHT;
				}
				break;
			case B_LEFT:
				if (x > 0 && objMoveFraction == 0)
				{
					startMove(D_LEFT);
					lastMoveDir = D_LEFT;
					faceDir = D_LEFT;
				}
				lastInput = B_LEFT;
				break;
			case B_DOWN:
				if (y < MAP_SIZE && objMoveFraction == 0)
				{
					startMove(D_DOWN);
					faceDir = D_DOWN;
					lastMoveDir = D_DOWN;
				}
				lastInput = B_DOWN;
				break;
			case B_UP:
				if (y > 0 && objMoveFraction == 0)
				{
					startMove(D_UP);
					faceDir = D_UP;
					lastMoveDir = D_UP;
				}
				lastInput = B_UP;
				break;
			default:
				//lastInput = 0;
				break;
			}
			posX = this->x;
			posY = this->y;
		}
	}