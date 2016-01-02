#include "objectDef.h"
#include "sprites.h"
#include "inputDef.h"
#include "level.h"
#include "debugText.h"
#include "imageMacros.h"

#include "recordMoves.h"
#include "tileEnum.h"

Object* player;
extern int pressureCount;
extern int prevMove;
extern double fpsModifier;
extern unsigned long lastInputTime;
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
extern bool replayEnabled;
int getInput(int index);
void cleanup();
void changeTextToDead();
class Level* getCurrentLevel();
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
unsigned long getTicks();
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
	int moveIndex;
	int inputMove;
	bool recordedMoveActive;
	Person(const Person &other, int _x, int _y)
	{
		memcpy(this, &other, sizeof(*this));
		x = _x;
		y = _y;
	}
	Person(int x2, int y2) : Object(x2, y2)
	{
		numFrames = 3;
		isPlayer = true;
		playerPlaced = true;
		playerDead = false;
		prevMove = D_NONE;
		active = true;
		moveIndex = 0;
	}
	void freeze() {
		Object::freeze();
		if (player == this)
			switchPlayerFocus();
	}
	//Moves, gets input if needed and checks for forces on the player eg conveyor belts
	void doLogic()
	{
		recordedMoveActive = false;
		inputMove = D_NONE;
		if (playerDead || displayName)
			return;
		if (active == false && objMoveDir == D_NONE)
			return;
		objMove();
		int input = INPUT_NONE;
		if (!replayEnabled || objMoveFraction == 0.0) {
			input = getInput(moveIndex);
			if (replayEnabled && input != INPUT_NONE) {
				moveIndex++;
			}
		}
		if (input != INPUT_NONE) {
			lastInputTime = getTicks();
		}
		if (frozen)
		{
			int tmp = 5;
		}
		switch (input){
		case BUTTON_1:
			if (lastInput != BUTTON_1)
			{
#ifdef DEBUG
				debugOn = !debugOn;
#endif
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
				inputMove = D_RIGHT;
				if (startMove(D_RIGHT, 1)) {
					faceDir = D_RIGHT;
					recordMove();
					recordedMoveActive = true;
				}
			}
			else if (!frozen)
			{
				lastInput = B_RIGHT;
				queuedMove = D_RIGHT;
			}
			break;
		case B_LEFT:
			if (x > 0 && objMoveFraction == 0 && !frozen)
			{
				inputMove = D_LEFT;
				if (startMove(D_LEFT, 1)) {
					faceDir = D_LEFT;
					recordMove();
					recordedMoveActive = true;
				}
			}
			else if (!frozen)
			{
				lastInput = B_LEFT;
				queuedMove = D_LEFT;
			}
			break;
		case B_DOWN:
			if (y < MAP_SIZE && objMoveFraction == 0 && !frozen)
			{
				inputMove = D_DOWN;
				if (startMove(D_DOWN, 1)) {
					faceDir = D_DOWN;
					recordMove();
					recordedMoveActive = true;
				}
			}
			else if (!frozen)
			{
				queuedMove = D_DOWN;
				lastInput = B_DOWN;
			}
			break;
		case B_UP:
			if (y > 0 && objMoveFraction == 0 && !frozen)
			{
				inputMove = D_UP;
				if (startMove(D_UP, 1)) {
					faceDir = D_UP;
					recordMove();
					recordedMoveActive = true;
				}
			}
			else if (!frozen)
			{
				queuedMove = D_UP;
				lastInput = B_UP;
			}
			break;
		default:
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
		return requestMove(x, y, checkX, checkY, this);
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
				Object::die();
			}
			else 
			{
				Object::die();
				return;
			}
			if (getCurrentLevel()->getTerrain(x, y)->id == m_water)
			{
				visible = false;
			}
		}
		else 
		{
			Object::die();
			return;
		}
	}
	Object* clone(int _x, int _y)
	{
		return new Person(*this, _x, _y);
	}
	bool startMove(int dir, int priority) override
	{
		if (priority < currentMovePriority)
			return false;
		if (recordedMoveActive)
		{
			recordedMoveActive = false;
			moveIndex--;
			unrecordMove();
		}
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
		if (dir == D_NONE || !requestMove(x, y, xChange, yChange, this))
			return false;
		objMoveDir = dir;
		tempSpeed = (double)3 * fpsModifier;
		return true;
	}
};
const char *Person::imageNames[4][3] = { { "gfx/personn1.png", "gfx/personn2.png", "gfx/personn3.png" },
{ "gfx/personw1.png", "gfx/personw2.png", "gfx/personw3.png" },
{ "gfx/persons1.png", "gfx/persons2.png", "gfx/persons3.png" },
{ "gfx/persone1.png", "gfx/persone2.png", "gfx/persone3.png" } };
SPRITE_MOVING(Person)