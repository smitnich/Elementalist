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
extern "C" {
	extern void __exception_setreload(int t);
}
bool smbInit();
#endif
extern string directorySymbol;
void parseConfig(char argv[]);
void musicInit();
void fileInit();
void inputInit();
void joystickInit();
void gfxInit();
bool fontInit();
void switchLevel(int level);
void clearObjects();
void freeSurface();
void checkEvents();
void drawScreen();
void objectLogic();
void doPlayer();
void doTerrainChanges();
void createGlobalInstances();
void freeGlobalInstances();
extern bool defaultPath, fullScreen, done, displayName;
extern string levelPath, appPath;
extern SDL_Surface *screen;
extern int videoSizeX, videoSizeY, bitDepth, argumentCount, frame, lastInput, \
framesPerSecond, levelChange, levelStartCounter;
unsigned long lastTicks = 0;
extern long lastInputTime;
double delta = 0.0;
extern bool playerDead;

void init(int argc, char* argv[])
{
	#ifdef GEKKO
	__exception_setreload(8);
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
	inputInit();
	#endif
	string tempFileName (argv[0]);
	//Determine which directory symbol to use
	if (tempFileName.find("\\") != string::npos)
		directorySymbol = "\\";
	else if (tempFileName.find("/") != string::npos)
		directorySymbol = "/";
	else
		exit(0);
	//Parse any arguments given

	if (argc > 1)
	{
		int i = 1;
		for (i = 1; i < argc; i++)
			parseConfig(argv[i]);
	}
	//If the path hasn't changed, generate the path from argv[0]
	if (defaultPath == 1)
	{
		if (tempFileName.rfind(directorySymbol) == string::npos)
			exit(0);
		tempFileName.resize(tempFileName.rfind(directorySymbol)+1);
		levelPath.append(directorySymbol);
		appPath.assign(tempFileName);
		//Change to the app directory, necessary for running levels outside of the folder
	//#ifndef GEKKO
		chdir(appPath.data());
	//#endif
	}
	musicInit();
	fileInit();
	if ( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
		SDL_Delay( 5000 );
		exit(EXIT_FAILURE);
	}
#ifndef GEKKO
	joystickInit();
	SDL_WM_SetCaption("Elementalist", 0 );
	SDL_WM_SetIcon(IMG_Load("gfx/icon_large.png"), NULL);
#endif
	if (fullScreen == false)
		screen = SDL_SetVideoMode(videoSizeX, videoSizeY, bitDepth, SDL_HWSURFACE|SDL_DOUBLEBUF);
	else
		screen = SDL_SetVideoMode(videoSizeX, videoSizeY, bitDepth, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	if ( !screen )
	{
		fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
		SDL_Delay( 5000 );
		exit(EXIT_FAILURE);
	}
	gfxInit();
	createGlobalInstances();
	fontInit();
	switchLevel(1);
	//fprintf(stderr, "\n\n\n\n\n");
	atexit(SDL_Quit);
	//parseDoc("levels/xml.ele");
#ifdef GEKKO
	SDL_ShowCursor(SDL_DISABLE);
#endif
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 91, 91, 255));
}
//Clear the screen and free all remaining resources
void cleanup(){
	done = true;
	//saveLevel(map,posX,posY);
#ifdef GEKKO
	fatUnmount(0);
#endif
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Flip(screen);
	Mix_CloseAudio();
	clearObjects();
	freeGlobalInstances();
	SDL_Quit();
	freeSurface();
#ifdef GEKKO
	MOUSE_Deinit();
	if (HWButton == -1)
		exit(EXIT_SUCCESS);
	else
		SYS_ResetSystem(HWButton,0,0);
#else
	exit(0);
#endif
}
//Find the left portion of an item on a given surface size
int getCenter(int surfaceSize, int itemSize)
{
	return ((surfaceSize/2)-(itemSize/2));
}
void updateDelta()
{
	unsigned int curTime = SDL_GetTicks();
	delta = ((double)(curTime - lastTicks)) / (1000.0 / ((float)framesPerSecond));
	lastTicks = curTime;
}
int main(int argc, char* argv[]){
	argumentCount = argc;
	init(argc, argv);
	//Main game loop
	while (!done)
	{
		updateDelta();
		checkEvents();
#ifdef GEKKO
		if (HWButton != -1)
			cleanup();
#endif
		if (displayName == false && playerDead == false)
			objectLogic();
		else
			doPlayer();
		doTerrainChanges();
		drawScreen();
		if (levelChange != -1)
		{
			switchLevel(levelChange);
			levelChange = -1;
		}
		SDL_Flip(screen);
		frame++;
	}
	return 0;
}