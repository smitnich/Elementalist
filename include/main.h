#include "base.h"
#include "sdlFiles.h"
#ifdef _WIN32
#include <direct.h>
#elif LINUX
#include <unistd.h>
#endif
#include "input_def.h"
//If wii
#ifdef GEKKO
s8 HWButton = -1;
#include <ogc/usbmouse.h>
#include "callback.h"
#endif
#include "tiles.h"
unsigned int lastTicks = 0;
double delta = 0.0;
void outputLog(char[]);
void outputLog(const char[]);
void outputLog(int);
void clearObjects();
void parseConfig(char[]);
void inputInit();
int getControlType();
int getInput();
void objectLogic();
bool checkWall(int x, int y, char map[MAP_SIZE][MAP_SIZE]);
void outputLog(int);
void musicInit();
extern Mix_Chunk *pushBlock;
bool playSound(Mix_Chunk *);
void drawScreen();
void gfxInit();
bool fontInit();
void fileInit();
bool smbInit();
void updatePointer();
void checkEvents();
void joystickInit();
void switchLevel(int);
string convertInt(int);
void updateDelta();
void freeSurface();
void doPlayer();
extern FILE *outputFile;
//The initial video resolution
//Can be overridden by config.txt or command line args
extern int videoSizeX;
extern int videoSizeY;
//The number of arguments passed into the Exe
extern int argumentCount;
//The number of pixels per tile
extern int tileSize;
//The number of tiles viewable horizontally and vertically, from the player
extern int tilesX;
extern int tilesY;
//The initial coordinates to begin drawing the game screen
extern int xInitial;
extern int yInitial;
//Used to switch to a particular level
extern int levelChange;
//The default starting position of the player
extern int posX;
extern int posY;
//The speed of conveyour belts
extern int conveyorSpeed;
//The default framerate of the game
//Can be overriden by config.txt or command line args
extern int framesPerSecond;
//The variable used to calculate the difference in movement speed
//based on the change in frames per second
extern double fpsModifier;
//The movespeed of the player character
extern double movespeed;
extern int levelStartCounter;
//The maximum rendering coordinates
//Can be overriden by config.txt or command line args
extern int xMax;
extern int yMax;
//The bitdepth, can be changed to 8bit for an odd effect
extern int bitDepth;
//The current level
//Set to 0 if a level is loaded via arg[1]
extern int currentLevel;
//The location where all other files are found from
//Usually argv[0]
extern string appPath;
//The location of the levels, appended onto the apppath
//Overridable via config.txt or command line args
extern string levelPath;
//Which directory symbol to use
//Found via searching argv[0] for the proper symbol
//If neither found, exit
extern string directorySymbol;
//The level to start with, used when passing in as an argument
extern string startLevel;
//The name of the level ie the filename minues the extension
extern string startLevelName;
//Whether or not to use the default executable path
extern bool defaultPath;
//Whether or not to use the default level path
extern bool defaultLevelPath;
//Whether to align the ui to the left instead of the right
extern bool alignLeft;
//Used to exit the main loop
extern bool done;
//Whether or not to display the level name
//Set to false when any input occurs
extern bool displayName;
//Whether or not to display in fullScreen
extern bool fullScreen;
//Set to true when the level is completed
//Shows a simple you won message
extern bool won;
//Used to time delta for the current frame
extern int lastInput;
extern int lastInputFrame;
extern int frame;
extern int prevFrame;
extern int curFrame;
extern string musicPath;