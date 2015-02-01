#ifndef _OBJECTDEF
#define _OBJECTDEF
#include "sdlFiles.h"
class Object;
class Object
{
public: int x, y,  objMoveDir, solid, pushable, frozen, numFrames, faceDir;
		double moveSpeed, tempSpeed, objMoveFraction;
		int prevMove;
		bool isPlayer;
		class Level *level;
		Object();
		SDL_Surface *spritens[6];
		SDL_Surface *spriteew[6];
		SDL_Surface *stationary;
		unsigned int lastTicks;
		int getMoveDir();
		double getMoveFraction();
		int getX();
		int getY();
		void startMove(int dir);
		virtual void die();
		virtual void doLogic();
		//Gets the sprite to be drawn on the screen
		SDL_Surface *getSprite();
		virtual bool requestEntry(Object* other, int dir);
};
class SolidObject : public Object {
public:
	void doLogic();
	SolidObject();
};
class Movable : public SolidObject{
public:
	Movable();
	void specialLogic();
	void doLogic();
	void objMove();
};
class Crate : public Movable{
public:
	Crate();
	Crate(int x, int y);
	void doLogic();
	bool requestEntry(Object* other, int dir);
};
class HeavyCrate : public Crate{
public:
	HeavyCrate();
	HeavyCrate(int x, int y);
	bool requestEntry(Object* other, int dir);
};
class IceBall : public Movable{
public:
	~IceBall();
	IceBall();
	IceBall(int x2, int y2, int moveDir, int moveFraction);
	void objMove();
	void doLogic();
	bool requestEntry(Object* other, int dir);
};
class Person : public Movable
{
public:
	int active;
	Person(int x, int y);
	void makeElement(bool doSecond);
	char element;
	void doLogic();
	bool requestEntry(Object* other, int dir);
	void die();
};
#endif