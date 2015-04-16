#ifndef IMAGE_MACRO
#include <list>
#define MAX_OBJECTS 1000
#define IMAGE_MACRO
void addToImageList(Object *other);
extern std::list<Object *> *imagesToLoad;
extern Object *objectList[MAX_OBJECTS];

#define SPRITE_STATIONARY(X,Y) SDL_Surface *X::stationary; \
SDL_Surface *X::spriteew[6]; \
SDL_Surface *X::spritens[6]; \
SDL_Surface *X::getSprite(){ \
	return X::stationary; \
} \
X::X(){ \
	if (imagesToLoad == NULL) \
		imagesToLoad = new std::list<Object *>(); \
	addToImageList(this); \
	if (objectList[objectNum-1000] == NULL) \
		objectList[objectNum-1000] = this; \
} \
void X::loadImages() \
{ \
	stationary = loadOptimizedIMG(Y); \
	for (int i = 0; i < 6; i++){ \
		spriteew[i] = stationary; \
		spritens[i] = stationary; \
				} \
}\
class X Test##X = X();
#define IMAGE_DECLARATION(X,Z) static SDL_Surface *stationary; \
static SDL_Surface *spriteew[6]; \
static SDL_Surface *spritens[6]; \
SDL_Surface *getSprite(); \
void loadImages();	\
X(); \
Object *createInstance(int x, int y) \
{ \
Object *tmp = new X(x, y); \
return tmp; \
} \
const static int objectNum = Z;

#define SPRITE_MOVING(X,Y) SDL_Surface *X::stationary; \
SDL_Surface *X::spriteew[6]; \
SDL_Surface *X::spritens[6]; \
SDL_Surface *X::getSprite(){ \
	return X::stationary; \
} \
X::X(){ \
	if (imagesToLoad == NULL) \
		imagesToLoad = new std::list<Object *>(); \
	addToImageList(this); \
} \
void X::loadImages() \
{ \
	stationary = loadOptimizedIMG(Y); \
	for (int i = 0; i < 6; i++){ \
		spriteew[i] = stationary; \
		spritens[i] = stationary; \
				} \
}\
class X Test##X = X();
#endif