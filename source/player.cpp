#include "objectDef.h"
#include "sprites.h"
#include "input_def.h"
#include "level.h"
#include "debugText.h"
extern int pressureCount;
extern int lastMoveDir;
extern unsigned int lastInputTime;
extern int frame;
extern bool playerPlaced;
extern bool displayName;
extern bool won;
extern int lastInput;
extern int currentLevelNum;
extern int levelChange;
extern std::string startLevelName;
void startMove(int dir);
extern int framesPerSecond;
extern int levelStartCounter;
bool playerDead = false;
int getInput();
void cleanup();
void changeTextToDead();
class Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
unsigned int getTicks();
extern double delta;
extern Object *player;
void gameOver();
bool switchPlayerFocus();
Object* objectInit(unsigned int id, int x, int y, int moveDir, int moveFraction);
//The player
class Person : public Object
{
public:
	OBJECT_DECLARATION(Person, 1004)
	int active;
	int lastMoveDir;
	Person(const Person &other, int _x, int _y)
	{
		active = true;
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
		queuedMove = D_NONE;
		for (int i = 0; i < 6; i++)
		{
			spriteew[i] = other.spriteew[i];
			spritens[i] = other.spritens[i];
		}
	}
	Person(int x2, int y2) : Object(x2,y2)
	{
		numFrames = 3;
		isPlayer = true;
		playerPlaced = true;
		playerDead = false;
		lastMoveDir = D_NONE;
	}
	void makeElement(bool doSecond)
	{
		return;
	}
	void freeze() {
		if (player == this)
			switchPlayerFocus();
		frozen = true;
	}
	//Moves, gets input if needed and checks for forces on the player eg conveyor belts
	void doLogic()
	{
		if (playerDead || displayName)
			return;
		if (active == false && objMoveDir == D_NONE)
			return;
		objMove();
		int input = getInput();
		if (input != INPUT_NONE)
			lastInputTime = getTicks();
		switch (input){
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
				startMove(D_RIGHT,1);
				lastMoveDir = D_RIGHT;
				faceDir = D_RIGHT;
			}
			lastInput = B_RIGHT;
			queuedMove = D_RIGHT;
			break;
		case B_LEFT:
			if (x > 0 && objMoveFraction == 0 && !frozen)
			{
				startMove(D_LEFT,1);
				lastMoveDir = D_LEFT;
				faceDir = D_LEFT;
			}
			lastInput = B_LEFT;
			queuedMove = D_LEFT;
			break;
		case B_DOWN:
			if (y < MAP_SIZE && objMoveFraction == 0 && !frozen)
			{
				startMove(D_DOWN,1);
				faceDir = D_DOWN;
				lastMoveDir = D_DOWN;
			}
			queuedMove = D_DOWN;
			lastInput = B_DOWN;
			break;
		case B_UP:
			if (y > 0 && objMoveFraction == 0 && !frozen)
			{
				startMove(D_UP,1);
				faceDir = D_UP;
				lastMoveDir = D_UP;
			}
			queuedMove = D_UP;
			lastInput = B_UP;
			break;
		default:
			//lastInput = B_NONE;
			break;
		}
	}	
	bool requestEntry(Object *other, int dir)
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
		if (!requestMove(x, y, checkX, checkY, this))
		{
			return false;
		}
		return true;
	}
	void onCollision(Object *other, int dir)
	{
		startMove(dir, 2);
	}
	void die()
	{
		if (player == this)
		{
			if (!switchPlayerFocus()) {
				gameOver();
			}
			else {
				delete this;
			}
		}
		else {
			delete this;
		}
	}
	Object* clone(int _x, int _y)
	{
		return new Person(*this, _x, _y);
	}
};
const char *Person::imageNames[4][3] = { { "gfx/personn1.png", "gfx/personn2.png", "gfx/personn3.png" },
{ "gfx/personw1.png", "gfx/personw2.png", "gfx/personw3.png" },
{ "gfx/persons1.png", "gfx/persons2.png", "gfx/persons3.png" },
{ "gfx/persone1.png", "gfx/persone2.png", "gfx/persone3.png" } };
SPRITE_MOVING(Person)