#include "sdlFiles.h"
#include "defs.h"
#include "object.h"
#include <iostream>
#ifdef _WIN32
#include <direct.h>
#elif LINUX
#include <unistd.h>
#endif
#include "inputDef.h"
//If wii
#ifdef GEKKO
#include "wii.h"
s8 HWButton = -1;
#include <ogc/usbmouse.h>
#include "callback.h"
extern "C" {
	extern void __exception_setreload(int t);
}
bool smbInit();
#endif

#include "player.h"
#include "files.h"
#include "gfx.h"
#include "event.h"
#include "inputManager.h"
#include "terrain.h"
#include "sound.h"
#include "queues.h"
#include "loadImg.h"
#include "input.h"
#include "levelSelect.h"
#include "level.h"

extern char directorySymbol;
extern bool defaultPath, fullScreen, done, displayName;
extern std::string levelPath, appPath, startLevel;
extern SDL_Surface *screen;
extern int videoSizeX, videoSizeY, bitDepth, argumentCount, frame, lastInput, \
framesPerSecond, levelChange, levelStartCounter;
extern unsigned long lastInputTime;
double delta = 0.0;
int lastInput = INPUT_NONE;
int currentScreen = SCR_LEVELSELECT;

int frame = 0;
unsigned long ticks = 0;
unsigned long lastTicks = 0;
unsigned long lastInputTime = 0;
extern char debugString[];
extern unsigned long levelStartTime;
//Keep every event that occurs during a frame to the same tick value
void setTicks()
{
	ticks = SDL_GetTicks();
}
unsigned long getTicks()
{
	return ticks;
}
void init(int argc, char* argv[])
{
	#ifdef GEKKO
	//__exception_setreload(8);
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
	inputInit();
	#endif
	std::string tempFileName (argv[0]);
	//Determine which directory symbol to use
	if (tempFileName.find("\\") != std::string::npos)
		directorySymbol = '\\';
	else
		directorySymbol = '/';
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
		if (tempFileName.rfind(directorySymbol) == std::string::npos)
			exit(0);
		tempFileName.resize(tempFileName.rfind(directorySymbol)+1);
		levelPath += directorySymbol;
		appPath.assign(tempFileName);
		//Change to the app directory, necessary for running levels outside of the folder
		chdir(appPath.data());
	}
	fileInit();
	if ( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
		SDL_Delay( 5000 );
		exit(EXIT_FAILURE);
	}
	if (fullScreen == false)
		screen = SDL_SetVideoMode(videoSizeX, videoSizeY, bitDepth, SDL_HWSURFACE | SDL_DOUBLEBUF);
	else
		screen = SDL_SetVideoMode(videoSizeX, videoSizeY, bitDepth, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	displayLoadingText();
	musicInit();
	SDL_ShowCursor(SDL_DISABLE);
#ifndef GEKKO
	joystickInit();
	SDL_WM_SetCaption("Elementalist", 0 );
	SDL_WM_SetIcon(IMG_Load("gfx/icon_large.png"), NULL);
#endif
	if ( !screen )
	{
		fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
		SDL_Delay( 5000 );
		exit(EXIT_FAILURE);
	}
	gfxInit();
	fontInit();
#ifdef GEKKO
	SDL_ShowCursor(SDL_DISABLE);
#else
	SDL_ShowCursor(SDL_ENABLE);
#endif
	makeLevelButtons();
	atexit(SDL_Quit);
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 91, 91, 255));
}
//Clear the screen and free all remaining resources
void cleanup() {
	done = true;
	//saveLevel(map,posX,posY);
#ifdef GEKKO
	fatUnmount(0);
#endif
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Flip(screen);
	Mix_CloseAudio();
	clearObjects();
	SDL_Quit();
	freeAllImages();
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
	unsigned int curTime = getTicks();
	delta = ((double)(curTime - lastTicks)) / (1000.0 / ((float)framesPerSecond));
	lastTicks = curTime;
}

void titleLoop();

int main(int argc, char* argv[]){
	argumentCount = argc;
	init(argc, argv);
	if (startLevel.length() == 0)
	{
		titleLoop();
		switchLevel(selectLevel());
	}
	else
	{
		loadLevel(startLevel, 0);
		playMusic(1);
	}
	//Main game loop
	while (!done)
	{
		setTicks();
		handleInput();
		updateDelta();
		checkEvents();
#ifdef GEKKO
		if (HWButton != -1)
			cleanup();
#endif
		if (displayName == false)
			objectLogic();
		else
			doPlayer();
		doTerrainChanges();
		drawScreen();
		if (levelChange != -1)
		{
			Mix_Volume(-1, 0);
			switchLevel(levelChange);
			levelChange = -1;
		}
		SDL_Flip(screen);
		frame++;
	}
	return 0;
}
void gameOver() {
	playerDead = true;
	changeTextToDead();
}