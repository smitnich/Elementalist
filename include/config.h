#include <iostream>
//These may be modified
//The location of the executable
extern std::string appPath;
//Whether the default path is used or not
extern bool defaultPath;
//The location of the levels
extern std::string levelPath;
//The starting portion of the game window from the left
extern int xInitial;
//Can be changed to 8bit for an interesting effect
extern int bitDepth;
//The resolution
extern int videoSizeY;
extern int videoSizeX;
//The value that makes objects move at the same relative speed regardless of
//the frames per second
extern double fpsModifier;
//The symbol used to indicate a folder, differs between operating systems
//Should be automatically detected using argv[0]
extern std::string directorySymbol;
//Whether or not to align the interface to the left or not
extern bool alignLeft;
//Wii SMB data for file sharing
#ifdef GEKKO
extern std::string smbUserName;
extern std::string smbPassword;
extern std::string smbFolderName;
extern std::string smbIp;
#endif
//The level to be loaded initially
//Changed when a .lev file is opened by the executable
extern std::string startLevel;
//Name of said file
extern std::string startLevelName;
//Whether or not to run the game in fullScreen
extern bool fullScreen;
//The refresh rate of the game
extern int framesPerSecond;
//The level to begin with, 0 = level loaded via opening it with the executable
extern int currentLevelNum;
//Number of tiles to be drawn
extern int tilesX;
extern int tilesY;
extern std::string musicPath;
void parseConfig(char input[]);
void loadLevelFromArg(std::string strConfig);