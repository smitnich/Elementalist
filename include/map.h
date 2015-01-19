#include "base.h"
#include "defs.h"
#include "level.h"
#include "sdlFiles.h"
#include <vector>
#include "objectDef.h"
Object* objectInit(char id, int x, int y);
//Used to make the levelname based on which number it is
string constructLevelName(int);
int pickWall(int x, int y);
void changeText();
Mix_Music* loadMusic(char *fileName);
//Whether or not the levelpath has been changed
bool changedPath = 0;
extern Mix_Music *levelMusic[MAX_LEVEL];
extern string levelPath;
extern string startLevel;
extern int posX;
extern int posY;
extern int currentLevelNum;