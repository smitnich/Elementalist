#ifndef _OBJECTDEF
#define _OBJECTDEF
#include "sdlFiles.h"
#include <stdexcept>
class Object
{
public: int x, y,  objMoveDir, solid, frozen, numFrames, faceDir;
		int id;
		double moveSpeed, tempSpeed, objMoveFraction;
		bool hovering;
		int queuedMove;
		int prevMove;
		double timeToLive, lifeTime;
		bool isPlayer;
		bool isMagnetic;
		int currentMovePriority;
		Object *within;
		virtual bool isMovableBlock();
		Object(int x, int y);
		Object();
		virtual ~Object();
		Object(Object &other, int x, int y);
		int getMoveDir();
		double getMoveFraction();
		int getX();
		int getY();
		bool objMove();
		bool startMove(int dir, int priority);
		virtual void drown()
		{
			die();
		}
		virtual void freeze()
		{
			frozen = true;
		}
		virtual void heat()
		{
			frozen = false;
		}
		virtual void electrocute()
		{
			if (!frozen)
				die();
		}
		//It should not be possible to have two different objects in one tile in
		//most cases, so throw an exception by default
		virtual void onEnter(Object *other, int xChange, int yChange) {
			throw std::logic_error("Multiple objects in one tile");
		}
		virtual bool allowEntry() {
			return false;
		}
		void preferLeftTurn();
		void preferRightTurn();
		virtual void draw(int moveFractionX, int moveFractionY);
		virtual void die();
		virtual void doLogic();
		void updateTime();
		virtual void update() {
			if (within != NULL)
				within->update();
			updateTime();
			doLogic();
		}
		virtual Object* clone(int x, int y) = 0;
		virtual void loadImages() = 0;
		//Gets the sprite to be drawn on the screen
		virtual SDL_Surface *getSprite() = 0;
		virtual bool requestEntry(Object *other, int dir);
		virtual void onCollision(Object *other, int dir) {
			return;
		}
		void setTimeToLive(double time) {
			timeToLive = time;
		}
		virtual Object* createInstance(int x, int y) = 0;
};
#include "imageMacros.h"
SDL_Surface* loadOptimizedIMG(const char *fileName);
#endif