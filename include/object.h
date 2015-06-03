#include "base.h"
#include "sdlFiles.h"
#include "sprites.h"
#include "input_def.h"
extern bool won;
//The base object class, all gameplay elements derive from this
class Object;
//The map which places Object pointers in their actual game locations, used for collision detection
extern int xInitial, yInitial, posX, posY, tilesX, tilesY;
extern int framesPerSecond;
extern int levelStartCounter;
extern double movespeed;
extern string levelName;
extern bool displayName;
extern int conveyorSpeed;
extern int levelChange;
extern string startLevelName;
int lastInput = INPUT_NONE;
extern int currentLevel;
//The current frame
int frame = 0;
//The last frame to recieve input
unsigned int lastInputTime = 0;
Object* player;
int lastMoveDir;
//Whether or not a player has been placed yet
//Used to determine which one to make active
bool playerPlaced = 0;
//The number of panels depressed
//Todo: Change to be more generic
int pressureCount = 0;
extern double fpsModifier;
//0 = not solid, 1 = solid 2 = solid & pushable
void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination);
void apply_surface(int, int, int, int, SDL_Surface*, SDL_Surface*);
void changeObjects(int);
void changeTextToWin();
bool between(int, int, int);
void cleanup();
SDL_Surface* pickSprite(int id, int dir, int moveFraction);
int compareCoordinates(Object*,Object*);
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY);
//Player stuff
int getInput();
//The last input to be recieved
void switchLevel(int);
void queuePlaceAll();
Object* objectInit(unsigned int id, int x, int y, int moveDir, int moveFraction);
