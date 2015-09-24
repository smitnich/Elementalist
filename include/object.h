#include "sdlFiles.h"
#include "sprites.h"
#include "inputDef.h"
#include <iostream>
extern bool won;
//The base object class, all gameplay elements derive from this
class Object;
//The map which places Object pointers in their actual game locations, used for collision detection
extern int xInitial, yInitial, tilesX, tilesY;
extern int framesPerSecond;
extern int levelStartCounter;
extern std::string levelName;
extern bool displayName;
extern int conveyorSpeed;
extern int levelChange;
extern std::string startLevelName;
extern int lastInput;
extern int currentLevel;
//The current frame
extern int frame;
//The last frame to recieve input
extern unsigned long lastInputTime;
//Whether or not a player has been placed yet
//Used to determine which one to make active
extern double fpsModifier;
//0 = not solid, 1 = solid 2 = solid & pushable
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
void clearObjects();
void objectDraw();
void objectLogic();
void doPlayer();
double getPlayerMoveFraction();
bool switchPlayerFocus();

void queuePlaceAll();
Object* objectInit(unsigned int id, int x, int y, int moveDir, int moveFraction);
