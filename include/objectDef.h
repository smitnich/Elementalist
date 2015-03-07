#ifndef _OBJECTDEF
#define _OBJECTDEF
#include "sdlFiles.h"
class Object
{
public: int x, y,  objMoveDir, solid, frozen, numFrames, faceDir;
		double moveSpeed, tempSpeed, objMoveFraction;
		int prevMove;
		bool isPlayer;
		class Level *level;
		virtual bool isMovableBlock();
		virtual bool isInsulated()
		{
			return false;
		}
		Object();
		Object(Object &other, int x, int y);
		SDL_Surface *spritens[6];
		SDL_Surface *spriteew[6];
		SDL_Surface *stationary;
		int getMoveDir();
		double getMoveFraction();
		int getX();
		int getY();
		void objMove();
		void startMove(int dir);
		virtual void freeze()
		{
			frozen = true;
		}
		virtual void die();
		virtual void doLogic();
		virtual Object* clone(int x, int y) = 0;
		//Gets the sprite to be drawn on the screen
		SDL_Surface *getSprite();
		virtual bool requestEntry(Object* other, int dir);
};
class Crate : public Object{
public:
	Crate();
	bool isInsulated()
	{
		return true;
	}
	bool isMovableBlock()
	{
		return true;
	};
	Crate(int x, int y);
	void doLogic();
	bool requestEntry(Object* other, int dir);
	Object *clone(int x, int y);
};
class HeavyCrate : public Crate{
public:
	HeavyCrate();
	HeavyCrate(int x, int y);
	bool requestEntry(Object* other, int dir);
	Object *clone(int x, int y);
};
class IceBall : public Object{
public:
	~IceBall();
	IceBall();
	IceBall(int x2, int y2, int moveDir, int moveFraction);
	void objMove();
	void doLogic();
	bool requestEntry(Object* other, int dir);
	Object *clone(int x, int y);
};
class Person : public Object
{
public:
	int active;
	Person(const Person &other, int x, int y);
	Person(int x, int y);
	void makeElement(bool doSecond);
	char element;
	void doLogic();
	bool requestEntry(Object* other, int dir);
	void die();
	Object *clone(int x, int y);
};
#endif