#include "base.h"
#include "sdlFiles.h"
#include "sprites.h"
#include "input_def.h"
extern bool won;
//The base object class, all gameplay elements derive from this
class Object;
//The map which places Object pointers in their actual game locations, used for collision detection
Object* objectInit(char id, int x, int y, int moveDir, int moveFraction);
extern int xInitial, yInitial, posX, posY, tilesX, tilesY, tileSize;
extern int framesPerSecond;
extern int levelStartCounter;
extern int pressureCount, totalPressureCount;
extern double movespeed;
extern string levelName;
extern bool displayName;
extern int conveyorSpeed;
extern int levelChange;
//Whether or not a player has been placed yet
//Used to determine which one to make active
bool playerPlaced = 0;
//The number of panels depressed
//Todo: Change to be more generic
int pressureCount = 0;
extern double fpsModifier;
char checkSolid(int xChange, int yChange);
char checkSolid(int x, int y, int xChange, int yChange);
//0 = not solid, 1 = solid 2 = solid & pushable
char solidArray[MAP_SIZE][MAP_SIZE];
void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination);
void apply_surface(int, int, int, int, SDL_Surface*, SDL_Surface*);
void outputLog(char[]);
void outputLog(const char[]);
void outputLog(int);
void changeObjects(int);
void changeTextToWin();
bool between(int, int, int);
void cleanup();
extern FILE* outputFile;
SDL_Surface* pickSprite(int id, int dir, int moveFraction);
void moveLine(int, int, int);
int compareCoordinates(Object*,Object*);
//Player stuff
int getInput();
//The last input to be recieved
int lastInput = 0;
extern int currentLevel;
void switchLevel(int);
extern string startLevelName;
//The current frame
int frame = 0;
//The last frame to recieve input
int lastInputFrame = 0;
Object* player;
int lastMoveDir;