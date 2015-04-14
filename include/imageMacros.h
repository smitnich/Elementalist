#ifndef IMAGE_MACRO
#include <list>
#define IMAGE_MACRO
void addToImageList(Object *other);
extern std::list<Object *> *imagesToLoad;

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
#define IMAGE_DECLARATION(X) static SDL_Surface *stationary; \
static SDL_Surface *spriteew[6]; \
static SDL_Surface *spritens[6]; \
SDL_Surface *getSprite(); \
void loadImages(); \
X();

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