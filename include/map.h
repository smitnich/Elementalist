#include "base.h"
#include "defs.h"
#include "level.h"
#include "sdlFiles.h"
#include <vector>
using namespace std;
char map[MAP_SIZE][MAP_SIZE];
int varMap[MAP_SIZE][MAP_SIZE];
void doVarMap();
class Object;
Object* objectInit(char id, int x, int y);
extern Object* objectMap[MAP_SIZE][MAP_SIZE];
//Used to make the levelname based on which number it is
string constructLevelName(int);
int pickWall(int x, int y);
void changeText();
Mix_Music* loadMusic(char *fileName);
//The total number of pressure features placed
//Todo: Make more generic
int totalPressureCount = 0;
extern bool playerPlaced;
//Whether or not the levelpath has been changed
bool changedPath = 0;
bool mapLoaded[MAX_LEVEL] = {0};
extern Mix_Music *levelMusic[MAX_LEVEL];
extern string levelPath;
extern string startLevel;
extern int posX;
extern int posY;
extern int currentLevelNum;