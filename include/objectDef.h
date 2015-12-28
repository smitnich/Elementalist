#ifndef _OBJECTDEF
#define _OBJECTDEF
#include "sdlFiles.h"
#include <stdexcept>
#include "defs.h"
#include "connection.h"
#include <vector>
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
		bool dead;
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
		bool visible;
		int totalConnections;
		int numConnectionsActive;
		std::vector<connection> connections;
		virtual bool startMove(int dir, int priority);
		void addConnection(Object *in, int _index);
		void addConnection(Terrain *in, int _index);
		virtual void drown()
		{
			die();
		}
		virtual void freeze()
		{
			frozen = true;
			objMoveDir = D_NONE;
			objMoveFraction = 0.0f;
		}
		virtual void heat(Object *heatObj = NULL)
		{
			frozen = false;
		}
		virtual void electrocute();
		virtual void burn();
		virtual void activate()
		{
			return;
		}
		virtual void deactivate()
		{
			return;
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
		void playSound(Mix_Chunk *sound);
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
		virtual bool slides() {
			return (frozen != 0);
		}
		virtual Object* createInstance(int x, int y) = 0;
};
#include "imageMacros.h"
SDL_Surface* loadOptimizedIMG(const char *fileName);
#endif