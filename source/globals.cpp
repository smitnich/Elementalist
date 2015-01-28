#include "base.h"
//The initial video resolution
//Can be overridden by config.txt or command line args
int videoSizeX = 640;
int videoSizeY = 480;
//The number of arguments passed into the Exe
int argumentCount = 0;
//The number of pixels per tile
int tileSize = 48;
//The number of tiles viewable horizontally and vertically, from the player
int tilesX = 4;
int tilesY = 4;
//The initial coordinates to begin drawing the game screen
int xInitial = 0;
int yInitial = 0;
//Used to switch to a particular level
int levelChange = -1;
//The default starting position of the player
int posX = 10;
int posY = 10;
//The default framerate of the game
//Can be overriden by config.txt or command line args
int framesPerSecond = 60;
//The variable used to calculate the difference in movement speed
//based on the change in frames per second
double fpsModifier = (double) 60/framesPerSecond;
//The movespeed of the player character
double movespeed = 3*fpsModifier;
int levelStartCounter = 0;
//The maximum rendering coordinates
//Can be overriden by config.txt or command line args
int xMax = 456;
int yMax = 456;
//The bitdepth, can be changed to 8bit for an odd effect
int bitDepth = 0;
//The current level
//Set to 0 if a level is loaded via arg[1]
int currentLevelNum = 1;
//The location where all other files are found from
//Usually argv[0]
string appPath;
//The location of the levels, appended onto the apppath
//Overridable via config.txt or command line args
string levelPath = "levels";
//Which directory symbol to use
//Found via searching argv[0] for the proper symbol
//If neither found, exit
string directorySymbol = "/";
//The level to start with, used when passing in as an argument
string startLevel = "";
//The name of the level ie the filename minues the extension
string startLevelName = "";
//Whether or not to use the default executable path
bool defaultPath = 1;
//Whether or not to use the default level path
bool defaultLevelPath = 1;
//Whether to align the ui to the left instead of the right
bool alignLeft = 0;
//Used to exit the main loop
bool done = false;
//Whether or not to display the level name
//Set to false when any input occurs
bool displayName = true;
//Whether or not to display in fullScreen
bool fullScreen = false;
//Set to true when the level is completed
//Shows a simple you won message
bool won = false;
int prevFrame = 0;
int curFrame = 0;
bool showCursor = false;