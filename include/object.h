#include "sdlFiles.h"
#include "sprites.h"
#include "inputDef.h"
#include <iostream>
//The base object class, all gameplay elements derive from this
class Object;

void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination);
void apply_surface(int, int, int, int, SDL_Surface*, SDL_Surface*);
void changeObjects(int);
void changeTextToWin();
bool between(int, int, int);
void cleanup();
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY);

void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY);
void doDraw(Object *drawObject, int xOffset, int yOffset);
Object* objectInit(unsigned int id, int x, int y);
void clearObjects();
void objectDraw();
void objectLogic();
void doPlayer();
double getPlayerMoveFraction();
bool switchPlayerFocus();
void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY);
bool checkCollision(Object *first, Object *second);
void queuePlaceAll();
bool objMove();
Object* objectInit(unsigned int id, int x, int y, int moveDir, int moveFraction);
