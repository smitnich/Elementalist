#include "inputDef.h"
#include "keyinput.h"
//USBKeyboard_IsConnected
extern bool won;
void doMouse(SDL_Event);
//The previous key hit
SDLKey oldKey = SDLK_FIRST;
//The key last hit to win
//Used to prevent the window from quickly closing after it is opened
SDLKey winningKey = SDLK_FIRST;
//The map of all possible keys
bool keyMap[SDLK_LAST] = {0};
//The number of keyspushed at a time
int keysPushed = 0;
//Todo: Allow configuration of the keymap
void updateKeys(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
		{
		keyMap[event.key.keysym.sym] = 1;
		oldKey = event.key.keysym.sym;
		keysPushed++;
		}
	else if (event.type == SDL_KEYUP)
		{
		keyMap[event.key.keysym.sym] = 0;
		oldKey = event.key.keysym.sym;
		keysPushed--;
		}
}
//Check if the exit button is pressed on the keyboard
//Make work regardless of chosen input
bool checkKeyBoard()
{
	if (keyMap[keyControls[keyMenu]] == 1)
		return 1;
	else
	return 0;
}
//Checks the key array in order to determine the proper action
int keyInput()
{
	if (checkKeyBoard() == 1)
		return BUTTON_MENU;
	if (won == true && keyMap[winningKey] == 1)
		return INPUT_NONE;
	if (keyMap[keyControls[keyMinimize]] == 1)
		SDL_WM_IconifyWindow();
	int leveldir = 0;
	int xdir = 0;
	int ydir = 0;
	if (keyMap[keyControls[keyButton1]])
		return BUTTON_1;
	if (keyMap[keyControls[keyButton2]])
		return BUTTON_2;
	if (keyMap[keyControls[keyNextLevel]]||keyMap[keyControls[altNextLevel]])
		leveldir += 1;
	if (keyMap[keyControls[keyPrevLevel]]||keyMap[keyControls[altPrevLevel]])
		leveldir -= 1;
	if (leveldir <= -1)
		return BUTTON_LEVEL_PREV;
	if (leveldir >= 1)
		return BUTTON_LEVEL_NEXT;
	if ((keyMap[keyControls[keyLeft]] && oldKey != keyControls[keyLeft])||(keyMap[keyControls[altLeft]]) && oldKey != keyControls[altLeft])
		xdir-=1;
	if ((keyMap[keyControls[keyRight]] && oldKey != keyControls[keyRight])||(keyMap[keyControls[altRight]] && oldKey != keyControls[altRight]))
		xdir += 1;
	if ((keyMap[keyControls[keyUp]] && oldKey != keyControls[keyUp])||(keyMap[keyControls[altUp]] && oldKey != keyControls[altUp]))
		ydir-=1;
	if ((keyMap[keyControls[keyDown]] && oldKey != keyControls[keyDown])||(keyMap[keyControls[altDown]] && oldKey != keyControls[altDown]))
		ydir += 1;
	if (ydir <= -1)
		return B_UP;
	if (ydir >= 1)
		return B_DOWN;
	if (xdir <= -1)
		return B_LEFT;
	if (xdir >= 1)
		return B_RIGHT;
	if (keyMap[keyControls[keyLeft]]||keyMap[keyControls[altLeft]])
		xdir-=1;
	if (keyMap[keyControls[keyRight]]||keyMap[keyControls[altRight]])
		xdir += 1;
	if (keyMap[keyControls[keyUp]]||keyMap[keyControls[altUp]])
		ydir-=1;
	if (keyMap[keyControls[keyDown]]||keyMap[keyControls[altDown]])
		ydir += 1;
	if (ydir <= -1)
		return B_UP;
	if (ydir >= 1)
		return B_DOWN;
	if (xdir <= -1)
		return B_LEFT;
	if (xdir >= 1)
		return B_RIGHT;
	if (keysPushed > 0)
		return B_ANY;
	return INPUT_NONE;
}
